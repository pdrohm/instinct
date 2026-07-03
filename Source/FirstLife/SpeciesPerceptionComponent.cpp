#include "SpeciesPerceptionComponent.h"

#include "AnimalCharacter.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "FirstLife.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAL/IConsoleManager.h"
#include "ScentFieldSubsystem.h"
#include "SpeciesPerceptionProfile.h"

namespace
{
	/** Species-switch camera settle speed: readable transition, not a cut. */
	constexpr float CameraInterpSpeed = 3.f;
	/** Bodies slower than this do not count as "moving" for motion/hearing senses. */
	constexpr float MotionSpeedThreshold = 60.f;
	/** SmellStrength (0..1) maps to this many cm of nose reach at 1.0. */
	constexpr float MaxScentReadRange = 4200.f;
	/** NightVisionStrength (0..1) maps to this much camera exposure bias at 1.0. */
	constexpr float MaxNightVisionExposureBias = 1.2f;
	/** The ambush focus cone (deg, total) inside which the big cat locks targets. */
	constexpr float AmbushFocusConeDegrees = 40.f;
	/** Ground overlays float this far above the floor so they never z-fight. */
	constexpr float GroundOffset = 6.f;
	constexpr int32 ArcStepDegrees = 8;

	/**
	 * Perceptual persistence: how long a target stays Tracked after the vision
	 * channel stops delivering it. This is edge-flicker damping, not memory — the
	 * Remembered rung (PERCEPTION_DESIGN.md §3) is a later slice. Kept short so it
	 * never becomes a silent wallhack; the honesty rule bends this far and no further.
	 */
	constexpr float PerceptualPersistenceSeconds = 0.4f;
	/** Where the eyes sit above the actor center for line-of-sight tests. */
	constexpr float EyeHeightOffset = 60.f;

	/**
	 * V1 overlays demoted (PERCEPTION_DESIGN.md §7): cones, markers, and scent blobs
	 * are a dev tool for inspecting what the knowledge model believes — never
	 * player-facing. The player-facing output is which actors exist on screen.
	 */
	TAutoConsoleVariable<int32> CVarPerceptionDebug(
		TEXT("firstlife.PerceptionDebug"), 0,
		TEXT("1 = draw species-perception debug overlays (vision cones, target markers, scent blobs)."));

	/** One frame's lifetime for every overlay primitive — redrawn each tick. */
	void DrawGroundArc(const UWorld& World, const FVector& Center, float Radius,
		float StartDeg, float EndDeg, const FColor& Color, float Thickness)
	{
		FVector Previous = Center + FVector(
			FMath::Cos(FMath::DegreesToRadians(StartDeg)) * Radius,
			FMath::Sin(FMath::DegreesToRadians(StartDeg)) * Radius, 0.f);
		for (float Deg = StartDeg + ArcStepDegrees; Deg <= EndDeg + KINDA_SMALL_NUMBER; Deg += ArcStepDegrees)
		{
			const float ClampedDeg = FMath::Min(Deg, EndDeg);
			const FVector Next = Center + FVector(
				FMath::Cos(FMath::DegreesToRadians(ClampedDeg)) * Radius,
				FMath::Sin(FMath::DegreesToRadians(ClampedDeg)) * Radius, 0.f);
			DrawDebugLine(&World, Previous, Next, Color, false, -1.f, 0, Thickness);
			Previous = Next;
		}
	}

	FColor StyleColor(EPerceptionOverlayStyle Style)
	{
		switch (Style)
		{
		case EPerceptionOverlayStyle::Prey:
			return FColor(120, 200, 255);
		case EPerceptionOverlayStyle::Scent:
			return FColor(255, 170, 60);
		case EPerceptionOverlayStyle::Ambush:
			return FColor(255, 90, 60);
		case EPerceptionOverlayStyle::Tactical:
		default:
			return FColor(200, 220, 200);
		}
	}
}

USpeciesPerceptionComponent::USpeciesPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpeciesPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();

	Profiles = TArray<TObjectPtr<USpeciesPerceptionProfile>>(
		USpeciesPerceptionProfile::BuildDebugProfiles(*this));

	if (const AAnimalCharacter* Animal = GetAnimalOwner())
	{
		if (const USpringArmComponent* Arm = Animal->GetCameraBoom())
		{
			BaseArmLength = Arm->TargetArmLength;
			BaseArmPitch = Arm->GetComponentRotation().Pitch;
		}
	}

	ApplyPostProcess();
}

void USpeciesPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InterpCamera(DeltaTime);

	// Knowledge gating belongs to the *inhabited* body only: single-viewport hiding
	// is global, so exactly one observer may drive it. AI bodies hide nothing, and
	// releasing possession (P) hands the world back in full.
	const AAnimalCharacter* Animal = GetAnimalOwner();
	if (Animal && Animal->IsPlayerControlled())
	{
		UpdateKnowledge(DeltaTime);
		if (CVarPerceptionDebug.GetValueOnGameThread() != 0)
		{
			DrawOverlays();
		}
	}
	else if (HiddenTargets.Num() > 0)
	{
		RestoreAllHidden();
	}
}

void USpeciesPerceptionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RestoreAllHidden();
	Super::EndPlay(EndPlayReason);
}

void USpeciesPerceptionComponent::UpdateKnowledge(float DeltaTime)
{
	AAnimalCharacter* Animal = GetAnimalOwner();
	const USpeciesPerceptionProfile* Profile = GetActiveProfile();
	UWorld* World = GetWorld();
	if (!Animal || !Profile || !World)
	{
		return;
	}

	for (TActorIterator<AAnimalCharacter> It(World); It; ++It)
	{
		AAnimalCharacter* Target = *It;
		if (!Target || Target == Animal)
		{
			continue;
		}

		float& SecondsUnseen = SecondsSinceSeen.FindOrAdd(Target);
		if (VisionChannelSees(*Animal, *Profile, *Target))
		{
			SecondsUnseen = 0.f;
		}
		else
		{
			SecondsUnseen += DeltaTime;
		}

		// Slice 1 has exactly two rungs: Tracked (the real actor) or Absent (nothing).
		SetTargetAbsent(*Target, SecondsUnseen > PerceptualPersistenceSeconds);
	}
}

bool USpeciesPerceptionComponent::VisionChannelSees(const AAnimalCharacter& Animal,
	const USpeciesPerceptionProfile& Profile, const AAnimalCharacter& Target) const
{
	const FVector Eye = Animal.GetActorLocation() + FVector(0.f, 0.f, EyeHeightOffset);
	const FVector TargetLocation = Target.GetActorLocation();
	const FVector Delta = TargetLocation - Animal.GetActorLocation();

	if (Delta.Size2D() > Profile.ForwardClarityDistance)
	{
		return false;
	}

	const FVector Facing = Animal.GetActorForwardVector().GetSafeNormal2D();
	const float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(
		FMath::Clamp(FVector::DotProduct(Facing, Delta.GetSafeNormal2D()), -1.f, 1.f)));
	if (AngleDeg > Profile.VisualFieldOfViewDegrees * 0.5f)
	{
		return false;
	}

	// Honesty rule: a target behind a rock is not seen, so it must not be shown.
	// Pawn capsules ignore the visibility channel, so only world geometry blocks.
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(SpeciesPerceptionLOS), /*bTraceComplex*/ false);
	TraceParams.AddIgnoredActor(&Animal);
	TraceParams.AddIgnoredActor(&Target);
	return !GetWorld()->LineTraceTestByChannel(Eye, TargetLocation, ECC_Visibility, TraceParams);
}

void USpeciesPerceptionComponent::SetTargetAbsent(AAnimalCharacter& Target, bool bAbsent)
{
	const TWeakObjectPtr<AAnimalCharacter> Key(&Target);
	if (bAbsent && !HiddenTargets.Contains(Key))
	{
		Target.SetActorHiddenInGame(true);
		HiddenTargets.Add(Key);
	}
	else if (!bAbsent && HiddenTargets.Contains(Key))
	{
		Target.SetActorHiddenInGame(false);
		HiddenTargets.Remove(Key);
	}
}

void USpeciesPerceptionComponent::RestoreAllHidden()
{
	for (const TWeakObjectPtr<AAnimalCharacter>& Hidden : HiddenTargets)
	{
		if (AAnimalCharacter* Target = Hidden.Get())
		{
			Target->SetActorHiddenInGame(false);
		}
	}
	HiddenTargets.Empty();
	SecondsSinceSeen.Empty();
}

void USpeciesPerceptionComponent::SetActiveProfileIndex(int32 Index)
{
	if (!Profiles.IsValidIndex(Index) || Index == ActiveProfileIndex)
	{
		return;
	}
	ActiveProfileIndex = Index;
	ApplyPostProcess();

	if (const USpeciesPerceptionProfile* Profile = GetActiveProfile())
	{
		UE_LOG(LogFirstLife, Log, TEXT("Perception switched to %s"), *Profile->SpeciesName.ToString());
	}
}

const USpeciesPerceptionProfile* USpeciesPerceptionComponent::GetActiveProfile() const
{
	return Profiles.IsValidIndex(ActiveProfileIndex) ? Profiles[ActiveProfileIndex].Get() : nullptr;
}

AAnimalCharacter* USpeciesPerceptionComponent::GetAnimalOwner() const
{
	return Cast<AAnimalCharacter>(GetOwner());
}

void USpeciesPerceptionComponent::ApplyPostProcess() const
{
	const AAnimalCharacter* Animal = GetAnimalOwner();
	const USpeciesPerceptionProfile* Profile = GetActiveProfile();
	if (!Animal || !Profile || !Animal->GetCamera())
	{
		return;
	}

	// Placeholder grade: global tint/vignette per style. Directional effects (a
	// desaturated periphery, a sharpened cone) need a post-process material — a
	// binary asset, deferred on purpose (SPECIES_PERCEPTION.md).
	float Saturation = 1.f;
	float Vignette = 0.25f;
	switch (Profile->OverlayStyle)
	{
	case EPerceptionOverlayStyle::Prey:
		Saturation = 0.8f;
		Vignette = 0.1f; // aware everywhere: the frame edge never closes in
		break;
	case EPerceptionOverlayStyle::Scent:
		Saturation = 0.55f; // dogs trade color for nose
		Vignette = 0.35f;
		break;
	case EPerceptionOverlayStyle::Ambush:
		Saturation = 0.8f;
		Vignette = 0.6f; // tunnel: the world narrows to what is ahead
		break;
	case EPerceptionOverlayStyle::Tactical:
	default:
		break;
	}

	FPostProcessSettings& PP = Animal->GetCamera()->PostProcessSettings;
	PP.bOverride_ColorSaturation = true;
	PP.ColorSaturation = FVector4(Saturation, Saturation, Saturation, 1.f);
	PP.bOverride_VignetteIntensity = true;
	PP.VignetteIntensity = Vignette;
	PP.bOverride_AutoExposureBias = true;
	PP.AutoExposureBias = Profile->NightVisionStrength * MaxNightVisionExposureBias;
}

void USpeciesPerceptionComponent::InterpCamera(float DeltaTime) const
{
	const AAnimalCharacter* Animal = GetAnimalOwner();
	const USpeciesPerceptionProfile* Profile = GetActiveProfile();
	if (!Animal || !Profile)
	{
		return;
	}
	USpringArmComponent* Arm = Animal->GetCameraBoom();
	if (!Arm || BaseArmLength <= 0.f)
	{
		return;
	}

	Arm->TargetArmLength = FMath::FInterpTo(Arm->TargetArmLength,
		BaseArmLength * Profile->CameraDistanceModifier, DeltaTime, CameraInterpSpeed);

	FRotator Rotation = Arm->GetComponentRotation();
	Rotation.Pitch = FMath::FInterpTo(Rotation.Pitch,
		BaseArmPitch * Profile->CameraHeightModifier, DeltaTime, CameraInterpSpeed);
	Arm->SetWorldRotation(Rotation);
}

void USpeciesPerceptionComponent::DrawOverlays() const
{
	const AAnimalCharacter* Animal = GetAnimalOwner();
	const USpeciesPerceptionProfile* Profile = GetActiveProfile();
	if (!Animal || !Profile || !GetWorld())
	{
		return;
	}

	DrawVisionArcs(*Animal, *Profile);
	DrawTargetMarkers(*Animal, *Profile);
	if (Profile->ScentVisibility > 0.f)
	{
		DrawScentOverlay(*Animal, *Profile);
	}
}

void USpeciesPerceptionComponent::DrawVisionArcs(const AAnimalCharacter& Animal,
	const USpeciesPerceptionProfile& Profile) const
{
	const UWorld& World = *GetWorld();
	const float CapsuleHalfHeight =
		Animal.GetCapsuleComponent() ? Animal.GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 90.f;
	const FVector Ground = Animal.GetActorLocation() - FVector(0.f, 0.f, CapsuleHalfHeight - GroundOffset);
	const float FacingYaw = Animal.GetActorRotation().Yaw;
	const float HalfFov = Profile.VisualFieldOfViewDegrees * 0.5f;
	const FColor Color = StyleColor(Profile.OverlayStyle);

	// Clear-vision cone: arc at clarity distance + the two cone edges.
	DrawGroundArc(World, Ground, Profile.ForwardClarityDistance,
		FacingYaw - HalfFov, FacingYaw + HalfFov, Color, 3.f);
	for (const float EdgeDeg : {FacingYaw - HalfFov, FacingYaw + HalfFov})
	{
		const FVector Edge(FMath::Cos(FMath::DegreesToRadians(EdgeDeg)),
			FMath::Sin(FMath::DegreesToRadians(EdgeDeg)), 0.f);
		DrawDebugLine(&World, Ground, Ground + Edge * Profile.ForwardClarityDistance,
			Color, false, -1.f, 0, 1.5f);
	}

	// The ambush focus cone: the narrow slice where the big cat actually hunts.
	if (Profile.OverlayStyle == EPerceptionOverlayStyle::Ambush)
	{
		const float HalfFocus = AmbushFocusConeDegrees * 0.5f;
		DrawGroundArc(World, Ground, Profile.ForwardClarityDistance * 0.98f,
			FacingYaw - HalfFocus, FacingYaw + HalfFocus, FColor(255, 220, 80), 5.f);
	}

	// Peripheral awareness ring: full circle — this sense has no facing.
	if (Profile.MotionDetectionStrength > 0.15f)
	{
		DrawGroundArc(World, Ground, Profile.PeripheralAwarenessDistance,
			0.f, 360.f, FColor(90, 90, 110), 1.f);
	}
}

void USpeciesPerceptionComponent::DrawTargetMarkers(const AAnimalCharacter& Animal,
	const USpeciesPerceptionProfile& Profile) const
{
	UWorld& World = *GetWorld();
	const FVector OwnLocation = Animal.GetActorLocation();
	const FVector Facing = Animal.GetActorForwardVector().GetSafeNormal2D();
	const float HalfFov = Profile.VisualFieldOfViewDegrees * 0.5f;
	const float Pulse = 0.5f + 0.5f * FMath::Sin(World.GetTimeSeconds() * 6.f);

	for (TActorIterator<AAnimalCharacter> It(&World); It; ++It)
	{
		const AAnimalCharacter* Target = *It;
		if (!Target || Target == &Animal)
		{
			continue;
		}

		const FVector Delta = Target->GetActorLocation() - OwnLocation;
		const float Distance = Delta.Size2D();
		const FVector Direction = Delta.GetSafeNormal2D();
		const float AngleDeg = FMath::RadiansToDegrees(
			FMath::Acos(FMath::Clamp(FVector::DotProduct(Facing, Direction), -1.f, 1.f)));
		const bool bMoving = Target->GetVelocity().Size2D() > MotionSpeedThreshold;
		const FVector MarkerPos = Target->GetActorLocation() + FVector(0.f, 0.f, 130.f);

		// Seen clearly: inside the cone and inside clarity range.
		if (AngleDeg <= HalfFov && Distance <= Profile.ForwardClarityDistance)
		{
			const bool bInFocus = Profile.OverlayStyle == EPerceptionOverlayStyle::Ambush &&
				AngleDeg <= AmbushFocusConeDegrees * 0.5f;
			if (bInFocus)
			{
				// Target lock: the ambush read is directional but decisive.
				DrawDebugBox(&World, MarkerPos, FVector(45.f), FColor(255, 220, 80), false, -1.f, 0, 4.f);
				DrawDebugLine(&World, OwnLocation, Target->GetActorLocation(),
					FColor(255, 220, 80), false, -1.f, 0, 2.f);
			}
			else
			{
				DrawDebugBox(&World, MarkerPos, FVector(35.f), StyleColor(Profile.OverlayStyle),
					false, -1.f, 0, 2.f);
			}
			continue;
		}

		// Motion-detected: outside clear vision, but it moved inside peripheral range.
		if (bMoving && Distance <= Profile.PeripheralAwarenessDistance &&
			Profile.MotionDetectionStrength > 0.f)
		{
			const float Radius = 30.f + 40.f * Pulse * Profile.MotionDetectionStrength;
			const bool bPrey = Profile.OverlayStyle == EPerceptionOverlayStyle::Prey;
			const FColor Alarm = bPrey ? FColor(255, 60, 60) : FColor(220, 200, 80);
			DrawDebugSphere(&World, MarkerPos, Radius, 10, Alarm, false, -1.f, 0, 2.f);
			if (bPrey)
			{
				// The deer's whole life: a line straight from you to the thing that moved.
				DrawDebugLine(&World, OwnLocation, Target->GetActorLocation(),
					Alarm, false, -1.f, 0, 1.5f);
			}
			continue;
		}

		// Heard only: a faint ring where sound places it, no visual identity.
		if (bMoving && Distance <= Profile.HearingStrength)
		{
			DrawDebugCircle(&World, Target->GetActorLocation(), 90.f + 50.f * Pulse, 16,
				FColor(150, 150, 160), false, -1.f, 0, 1.f,
				FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f), false);
		}
	}
}

void USpeciesPerceptionComponent::DrawScentOverlay(const AAnimalCharacter& Animal,
	const USpeciesPerceptionProfile& Profile) const
{
	UWorld& World = *GetWorld();
	const UScentFieldSubsystem* ScentField = World.GetSubsystem<UScentFieldSubsystem>();
	if (!ScentField)
	{
		return;
	}

	const FVector OwnLocation = Animal.GetActorLocation();
	const float NoseRange = Profile.SmellStrength * MaxScentReadRange;
	const float MaxAge = ScentField->GetMaxAgeSeconds();

	for (const FScentPoint& Point : ScentField->GetScentPoints())
	{
		const FVector Where = ScentField->GetDriftedLocation(Point);
		if (FVector::Dist2D(Where, OwnLocation) > NoseRange)
		{
			continue;
		}
		const float Freshness = 1.f - FMath::Clamp(ScentField->GetAgeSeconds(Point) / MaxAge, 0.f, 1.f);
		// Fresh scent burns hot orange; old scent cools to blue-grey and shrinks.
		const FColor Blob = FMath::Lerp(FLinearColor(0.25f, 0.3f, 0.45f),
			FLinearColor(1.f, 0.5f, 0.1f), Freshness).ToFColor(true);
		DrawDebugSphere(&World, Where, 16.f + 26.f * Freshness, 6, Blob, false, -1.f, 0,
			1.f + 2.f * Freshness * Profile.ScentVisibility);
	}

	// Wind: the invisible fact every real tracker lives by.
	const FVector ArrowBase = OwnLocation + FVector(0.f, 0.f, 220.f);
	DrawDebugDirectionalArrow(&World, ArrowBase,
		ArrowBase + ScentField->GetWindDirection() * 260.f, 120.f,
		FColor(80, 220, 255), false, -1.f, 0, 3.f);
}
