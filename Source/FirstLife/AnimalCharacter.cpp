#include "AnimalCharacter.h"

#include "AnimalAIController.h"
#include "AnimalConfig.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "FirstLife.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HungerComponent.h"
#include "LocomotionComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SpeciesPerceptionComponent.h"
#include "StaminaComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	// Fixed isometric rig (D14). Angle/distance/FOV are first-feel defaults for the
	// open camera questions (Q15 distance, Q16 rotation) — provisional until the
	// hands-on pass, like the locomotion numbers in D13.
	constexpr float IsoCameraPitch = -52.5f;
	constexpr float IsoCameraYaw = -45.f;
	constexpr float IsoCameraDistance = 2600.f;
	constexpr float IsoCameraFieldOfView = 45.f;
	constexpr float IsoCameraLagSpeed = 8.f;

	// --- Posture telegraph tuning (H14) — all first-pass guesses, tune by feel ---
	// How much of the head's base height it sheds at full fatigue. 0.7 = nose drops to
	// ~30% of its standing height when the animal is completely blown.
	constexpr float FatigueHeadDropFraction = 0.7f;
	// How far forward (species +X, "nose out") the head eases at full fatigue, cm.
	constexpr float FatigueHeadForward = 18.f;
	// Interp speed for the alive fatigue lerp — gentle, so the head sinks as it tires.
	constexpr float FatigueInterpSpeed = 4.f;
	// --- Downed collapse tuning (H14) ---
	// How far the body sinks in Z when it falls, cm (grey-box hitting the ground).
	constexpr float DownedBodyDrop = 55.f;
	// Roll applied to the whole mesh stack so the box visibly topples, degrees.
	constexpr float DownedRollDegrees = 55.f;
	// Extra head drop on collapse, cm — nose all the way to the dirt.
	constexpr float DownedHeadDrop = 60.f;
	// Interp speed for the collapse — snappier than fatigue so it buckles, not floats.
	constexpr float DownedInterpSpeed = 6.f;

	// --- Straggler condition (H14) — first-pass, tune by feel ---
	// The worst body condition a spawned individual can roll. 0.78 = a straggler sustains
	// ~22% less before it blows — catchable within one chase, not obviously crippled.
	// Grounded in predator prey-selection (Mech/Peterson; FitzGibbon): predators crop the
	// substandard, prime adults escape. The spawner weights rolls near 1.0 (few stragglers).
	constexpr float ConditionFloor = 0.78f;
	constexpr float GroundSnapTraceUp = 20000.f;
	constexpr float GroundSnapTraceDown = 50000.f;
	constexpr float GroundSnapSlack = 3.f;
}

AAnimalCharacter::AAnimalCharacter()
{
	// Movement thinks in ULocomotionComponent's tick, but the body itself now ticks to
	// drive the fatigue/downed posture telegraph (H14): head-drop off stamina, collapse
	// on catch. Pure cosmetic lerps — no gameplay authority lives here.
	PrimaryActorTick.bCanEverTick = true;

	// Humanoid proportions: ~180cm standing figure.
	GetCapsuleComponent()->InitCapsuleSize(34.f, 90.f);

	// The body faces where it runs; the camera is free to look around it.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
	Hunger = CreateDefaultSubobject<UHungerComponent>(TEXT("Hunger"));
	Locomotion = CreateDefaultSubobject<ULocomotionComponent>(TEXT("Locomotion"));
	Perception = CreateDefaultSubobject<USpeciesPerceptionComponent>(TEXT("Perception"));

	// Grey-box early human: an upright torso box and a head box, from engine content
	// only. Placeholder until a marketplace/Fab humanoid is brought in (spec: art is
	// acquired, never modeled).
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -14.f));
	BodyMesh->SetRelativeScale3D(FVector(0.38f, 0.5f, 1.44f));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(4.f, 0.f, 74.f));
	HeadMesh->SetRelativeScale3D(FVector(0.24f, 0.24f, 0.26f));

	// ACharacter already owns a SkeletalMeshComponent named Mesh. Keep it dormant
	// until a species config supplies imported art; the grey-box cubes remain the
	// fallback path for data-only species.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetHiddenInGame(true);
	GetMesh()->SetVisibility(false, true);

	if (CubeMesh.Succeeded())
	{
		GreyBoxMesh = CubeMesh.Object;
		BodyMesh->SetStaticMesh(CubeMesh.Object);
		HeadMesh->SetStaticMesh(CubeMesh.Object);
	}

	// Fixed isometric eye over the fully 3D scene (D14). Absolute rotation: the body
	// turns to face where it runs, the camera never turns with it — no orbit, no zoom,
	// no tactical controls yet (Q15/Q16). The narrow FOV at long range flattens
	// perspective toward the isometric read; spring-arm lag keeps the follow smooth.
	// Collision test off: terrain between eye and body must never snap-zoom the view —
	// occlusion handling is deferred with Q16.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(IsoCameraPitch, IsoCameraYaw, 0.f));
	SpringArm->TargetArmLength = IsoCameraDistance;
	SpringArm->SocketOffset = FVector::ZeroVector;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = IsoCameraLagSpeed;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetFieldOfView(IsoCameraFieldOfView);

	// The agent's own brain. Only spawned when no player holds the body (H7 seam).
	AIControllerClass = AAnimalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	ConfigAsset = TSoftObjectPtr<UAnimalConfig>(
		FSoftObjectPath(TEXT("/Game/Agents/DA_Human.DA_Human")));
}

void AAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Herd-spawn path injects a runtime species config (ADR-E4: one pawn, many species);
	// otherwise resolve the per-instance ConfigAsset (defaults to DA_Human).
	ResolvedConfig = ConfigOverride ? ConfigOverride.Get() : ConfigAsset.LoadSynchronous();
	if (!ResolvedConfig)
	{
		ResolvedConfig = GetDefault<UAnimalConfig>();
		UE_LOG(LogFirstLife, Log,
			TEXT("%s: no config asset found, using UAnimalConfig class defaults"), *GetName());
	}

	ApplyConfig();
}

void AAnimalCharacter::ApplyConfig()
{
	// Per-agent condition (H14 straggler): the spawner rolls it near 1.0 with a thin tail to
	// a substandard floor; the player body keeps 1.0. It FULLY scales the innate stamina
	// reserve (a compromised animal has less to spend) and LIGHTLY trims top speed
	// (0.5 + 0.5·condition), so a straggler runs with the herd on the first flush but empties
	// sooner and drifts to the rear under pressure — the readable target the hunt is won on.
	Condition = FMath::Clamp(Condition, ConditionFloor, 1.f);
	Stamina->Configure(ResolvedConfig->MaxStamina * Condition, ResolvedConfig->ExhaustionRecoveryFraction);
	Hunger->Configure(ResolvedConfig->MaxHunger, ResolvedConfig->HungerDrainPerSecond);
	Locomotion->Configure(*ResolvedConfig);
	Locomotion->SetSpeedScale(0.5f + 0.5f * Condition);

	// The species shapes its own grey-box body: the constructor's human cubes are
	// only defaults, re-proportioned here from data (ADR-E4 extended to silhouette).
	// Capsule first — mesh offsets are relative to its center. Spawns drop in from
	// above (Z + slack), so a shorter species settles onto the floor, never inside it.
	GetCapsuleComponent()->SetCapsuleSize(ResolvedConfig->CapsuleRadius, ResolvedConfig->CapsuleHalfHeight);
	BodyMesh->SetRelativeRotation(FRotator::ZeroRotator);
	HeadMesh->SetRelativeRotation(FRotator::ZeroRotator);
	BodyMesh->SetRelativeLocation(ResolvedConfig->BodyOffset);
	BodyMesh->SetRelativeScale3D(ResolvedConfig->BodyScale);
	HeadMesh->SetRelativeLocation(ResolvedConfig->HeadOffset);
	HeadMesh->SetRelativeScale3D(ResolvedConfig->HeadScale);

	if (USkeletalMesh* VisualMesh = ResolvedConfig->VisualSkeletalMesh.LoadSynchronous())
	{
		USkeletalMeshComponent* CharacterMesh = GetMesh();
		CharacterMesh->SetSkeletalMesh(VisualMesh);
		CharacterMesh->SetRelativeLocation(ResolvedConfig->VisualMeshOffset);
		CharacterMesh->SetRelativeRotation(ResolvedConfig->VisualMeshRotation);
		CharacterMesh->SetRelativeScale3D(ResolvedConfig->VisualMeshScale);
		CharacterMesh->SetHiddenInGame(false);
		CharacterMesh->SetVisibility(true, true);

		UClass* AnimClass = ResolvedConfig->VisualAnimClass.LoadSynchronous();
		CharacterMesh->SetAnimInstanceClass(AnimClass);

		BodyMesh->SetHiddenInGame(true);
		HeadMesh->SetHiddenInGame(true);
		BodyMesh->SetVisibility(false, true);
		HeadMesh->SetVisibility(false, true);

		UE_LOG(LogFirstLife, Log, TEXT("%s: using visual skeletal mesh %s"),
			*GetName(), *ResolvedConfig->VisualSkeletalMesh.ToSoftObjectPath().ToString());
	}
	else
	{
		if (!ResolvedConfig->VisualSkeletalMesh.IsNull())
		{
			UE_LOG(LogFirstLife, Warning, TEXT("%s: failed to load visual skeletal mesh %s; using grey-box fallback"),
				*GetName(), *ResolvedConfig->VisualSkeletalMesh.ToSoftObjectPath().ToString());
		}

		GetMesh()->SetSkeletalMesh(nullptr);
		GetMesh()->SetHiddenInGame(true);
		GetMesh()->SetVisibility(false, true);

		if (UStaticMesh* VisualStaticMesh = ResolvedConfig->VisualStaticMesh.LoadSynchronous())
		{
			BodyMesh->SetStaticMesh(VisualStaticMesh);
			BodyMesh->SetRelativeLocation(ResolvedConfig->VisualMeshOffset);
			BodyMesh->SetRelativeRotation(ResolvedConfig->VisualMeshRotation);
			BodyMesh->SetRelativeScale3D(ResolvedConfig->VisualMeshScale);
			BodyMesh->SetHiddenInGame(false);
			BodyMesh->SetVisibility(true, true);

			HeadMesh->SetHiddenInGame(true);
			HeadMesh->SetVisibility(false, true);

			UE_LOG(LogFirstLife, Log, TEXT("%s: using visual static mesh %s"),
				*GetName(), *ResolvedConfig->VisualStaticMesh.ToSoftObjectPath().ToString());
		}
		else
		{
			if (!ResolvedConfig->VisualStaticMesh.IsNull())
			{
				UE_LOG(LogFirstLife, Warning, TEXT("%s: failed to load visual static mesh %s; using grey-box fallback"),
					*GetName(), *ResolvedConfig->VisualStaticMesh.ToSoftObjectPath().ToString());
			}

			BodyMesh->SetStaticMesh(GreyBoxMesh);
			HeadMesh->SetStaticMesh(GreyBoxMesh);
			BodyMesh->SetHiddenInGame(false);
			HeadMesh->SetHiddenInGame(false);
			BodyMesh->SetVisibility(true, true);
			HeadMesh->SetVisibility(true, true);
		}
	}

	// Remember the species' neutral head/body positions: the posture telegraph in Tick()
	// is expressed as a displacement from these, so a short reindeer and a tall human
	// each drop their nose relative to their own silhouette, not a shared constant.
	HeadBaseOffset = HeadMesh->GetRelativeLocation();
	BodyBaseOffset = BodyMesh->GetRelativeLocation();
	HeadBaseRotation = HeadMesh->GetRelativeRotation();
	BodyBaseRotation = BodyMesh->GetRelativeRotation();

	// Tint both boxes via a dynamic instance of the engine BasicShapeMaterial
	// (exposes a "Color" vector parameter). One MID per component: they share the
	// parent material, and a shared MID would repaint every species at once.
	for (UStaticMeshComponent* Box : { BodyMesh.Get(), HeadMesh.Get() })
	{
		if (UMaterialInstanceDynamic* Tint = Box->CreateAndSetMaterialInstanceDynamic(0))
		{
			Tint->SetVectorParameterValue(TEXT("Color"), ResolvedConfig->BodyColor);
		}
	}

	SnapToGround();
}

void AAnimalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Metabolism + its coupling to stamina (H14). Hunger is the PLAYER's survival pressure,
	// so it only ticks — and only presses on stamina — while a human inhabits this body.
	// The instant it's AI-driven (released, or the whole reindeer herd) hunger freezes and
	// the modifiers snap back to neutral, so no AI body is ever starvation-penalized.
	if (Hunger && Stamina)
	{
		if (IsPlayerControlled())
		{
			Hunger->Update(DeltaTime);
			Stamina->SetHungerModifiers(Hunger->GetStaminaCapMult(), Hunger->GetStaminaRegenMult());
		}
		else
		{
			Stamina->SetHungerModifiers(1.f, 1.f);
		}
	}

	// Nothing to pose until BeginPlay has resolved the config and captured base offsets.
	if (!ResolvedConfig || !BodyMesh || !HeadMesh)
	{
		return;
	}

	FVector HeadTarget = HeadBaseOffset;
	FVector BodyTarget = BodyBaseOffset;
	float RollTarget = 0.f;
	float InterpSpeed = FatigueInterpSpeed;

	if (bDowned)
	{
		// Collapse pose: the whole silhouette sinks and rolls onto its side, nose to the
		// dirt. Overrides fatigue entirely — a caught animal is on the ground, not tiring.
		BodyTarget.Z -= DownedBodyDrop;
		HeadTarget.Z -= DownedHeadDrop;
		RollTarget = DownedRollDegrees;
		InterpSpeed = DownedInterpSpeed;
	}
	else if (const UStaminaComponent* Stam = GetStamina())
	{
		// CONTINUOUS with fatigue (H14): the nose sinks and eases forward as stamina
		// bleeds, so the "this one is blowing" tell grows smoothly into the catch window
		// rather than popping on at some threshold. Drives off 1 − staminaFraction.
		const float Fatigue = FMath::Clamp(1.f - Stam->GetStaminaFraction(), 0.f, 1.f);
		HeadTarget.Z -= HeadBaseOffset.Z * FatigueHeadDropFraction * Fatigue;
		HeadTarget.X += FatigueHeadForward * Fatigue;
	}

	// Ease the live mesh offsets toward the target pose — flow, never snap (FInterpTo).
	HeadMesh->SetRelativeLocation(
		FMath::VInterpTo(HeadMesh->GetRelativeLocation(), HeadTarget, DeltaTime, InterpSpeed));
	BodyMesh->SetRelativeLocation(
		FMath::VInterpTo(BodyMesh->GetRelativeLocation(), BodyTarget, DeltaTime, InterpSpeed));

	// Roll only bites on collapse; ease both boxes so the grey-box visibly buckles over.
	const float RollNow = FMath::FInterpTo(
		BodyMesh->GetRelativeRotation().Roll, RollTarget, DeltaTime, InterpSpeed);
	BodyMesh->SetRelativeRotation(BodyBaseRotation + FRotator(0.f, 0.f, RollNow));
	HeadMesh->SetRelativeRotation(HeadBaseRotation + FRotator(0.f, 0.f, RollNow));
}

void AAnimalCharacter::SnapToGround()
{
	UWorld* World = GetWorld();
	const UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!World || !Capsule)
	{
		return;
	}

	const FVector Location = GetActorLocation();
	const FVector Start = Location + FVector(0.f, 0.f, GroundSnapTraceUp);
	const FVector End = Location - FVector(0.f, 0.f, GroundSnapTraceDown);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AnimalGroundSnap), /*bTraceComplex=*/true);
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FHitResult Hit;
	if (World->LineTraceSingleByObjectType(Hit, Start, End, ObjectParams, Params))
	{
		const float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
		SetActorLocation(
			FVector(Location.X, Location.Y, Hit.ImpactPoint.Z + CapsuleHalfHeight + GroundSnapSlack),
			false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		UE_LOG(LogFirstLife, Warning,
			TEXT("%s: ground snap found no WorldStatic surface under spawn; keeping Z=%0.1f"),
			*GetName(), Location.Z);
	}
}

void AAnimalCharacter::SetRuntimeConfig(const UAnimalConfig* InConfig)
{
	if (!InConfig)
	{
		return;
	}

	ConfigOverride = InConfig;
	ResolvedConfig = InConfig;
	Condition = 1.f;
	ApplyConfig();
}

void AAnimalCharacter::SetDowned(bool bInDowned)
{
	// State only — the collapse is cosmetic (Tick) and the brain-silence is enforced by
	// the AI controller reading IsDowned(). The HuntSubsystem is the sole caller (H14).
	bDowned = bInDowned;
}

void AAnimalCharacter::SetWantsToSprint(bool bInWantsToSprint)
{
	Locomotion->SetWantsToSprint(bInWantsToSprint);
}

void AAnimalCharacter::SetWantsToWalk(bool bInWantsToWalk)
{
	Locomotion->SetWantsToWalk(bInWantsToWalk);
}

bool AAnimalCharacter::IsSprinting() const
{
	return Locomotion->IsSprinting();
}

float AAnimalCharacter::GetCameraYaw() const
{
	return SpringArm->GetComponentRotation().Yaw;
}
