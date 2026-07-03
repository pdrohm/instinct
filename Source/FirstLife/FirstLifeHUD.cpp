#include "FirstLifeHUD.h"

#include "AnimalCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "EngineUtils.h"
#include "HungerComponent.h"
#include "HuntSubsystem.h"
#include "LocomotionComponent.h"
#include "ScentFieldSubsystem.h"
#include "SpeciesPerceptionComponent.h"
#include "SpeciesPerceptionProfile.h"
#include "StaminaComponent.h"

namespace
{
	const TCHAR* GaitLabel(const ULocomotionComponent& Locomotion)
	{
		if (Locomotion.IsResting())
		{
			return TEXT("RESTING");
		}
		switch (Locomotion.GetActiveGait())
		{
		case ELocomotionGait::Walk:
			return TEXT("WALK");
		case ELocomotionGait::Sprint:
			return TEXT("SPRINT");
		case ELocomotionGait::Jog:
		default:
			return TEXT("JOG");
		}
	}
}

namespace
{
	constexpr float BarWidth = 420.f;
	constexpr float BarHeight = 16.f;
	constexpr float BarBottomMargin = 64.f;
	constexpr float LowStaminaFraction = 0.3f;

	// --- Hunger bar (H14): the survival meter, stacked directly above stamina, same
	// width so the two reservoirs read as one column. Player body only — an AI never
	// starves, so it never shows a hunger bar. ---
	constexpr float HungerBarHeight = 10.f;
	// Vertical gap from the top of the stamina bar up to the bottom of the hunger bar.
	constexpr float HungerBarGap = 10.f;
	// Warm amber when fed, shifting toward alarm-red as the gut empties — colour language,
	// distinct from stamina's bone-white so the two meters never read as the same thing.
	const FLinearColor HungerFullColor(0.95f, 0.62f, 0.12f);
	const FLinearColor HungerEmptyColor(0.85f, 0.16f, 0.1f);

	// --- Hunt readout layout (H14), top-center block ---
	constexpr float HuntBlockTopMargin = 24.f;
	// Rough half-width of the text so the line reads centered without measuring glyphs.
	constexpr float HuntTextHalfWidth = 90.f;
	// How long the "DOWNED — fed" banner lingers after a kill before the HUD hides it, s.
	constexpr float KillBannerSeconds = 4.f;
	// The "closing in" catch-progress bar, drawn under the live hunt clock.
	constexpr float HuntBarWidth = 200.f;
	constexpr float HuntBarHeight = 8.f;
	constexpr float HuntBarTopGap = 20.f;

	// --- Spoor tracking cue (H14) — the tracking half of the hunt (Liebenberg). ---
	// Beyond this distance to the nearest live prey, the hunter has lost close contact and
	// must follow the trail rather than the animal; inside it, the quarry is in view and the
	// cue hides. First-pass guess (cm).
	constexpr float TrackingRevealDistance = 2500.f;
	// How far out the hunter can pick up a trail to read its freshest point (cm). First-pass.
	constexpr float TrackingSniffRadius = 3500.f;
	// Spoor arrow geometry, drawn center-screen: shaft length and arrowhead length (px).
	constexpr float SpoorArrowLength = 70.f;
	constexpr float SpoorArrowHeadLength = 16.f;
	// Fraction of screen height the arrow is centered at (below the top hunt block).
	constexpr float SpoorScreenHeightFraction = 0.42f;
	// Freshness colour: a cold trail reads faint blue-grey, a hot one burns orange.
	const FLinearColor SpoorColdColor(0.45f, 0.55f, 0.7f);
	const FLinearColor SpoorHotColor(0.95f, 0.55f, 0.15f);
	// Freshness word thresholds.
	constexpr float SpoorHotThreshold = 0.66f;
	constexpr float SpoorWarmThreshold = 0.33f;
}

void AFirstLifeHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas || !PlayerOwner)
	{
		return;
	}

	// While the player spectates (AI has the body), keep reading the viewed animal.
	AAnimalCharacter* Animal = Cast<AAnimalCharacter>(PlayerOwner->GetPawn());
	const bool bInhabited = Animal != nullptr;
	if (!Animal)
	{
		Animal = Cast<AAnimalCharacter>(PlayerOwner->GetViewTarget());
	}
	if (!Animal || !Animal->GetStamina())
	{
		return;
	}

	const UStaminaComponent* Stamina = Animal->GetStamina();
	const float Fraction = Stamina->GetStaminaFraction();
	const bool bExhausted = Stamina->IsExhausted();

	const float X = (Canvas->SizeX - BarWidth) * 0.5f;
	const float Y = Canvas->SizeY - BarBottomMargin;

	DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.55f), X - 3.f, Y - 3.f, BarWidth + 6.f, BarHeight + 6.f);

	FLinearColor Fill(0.92f, 0.9f, 0.82f);
	if (bExhausted)
	{
		Fill = FLinearColor(0.85f, 0.2f, 0.15f);
	}
	else if (Fraction < LowStaminaFraction)
	{
		Fill = FLinearColor(0.95f, 0.65f, 0.15f);
	}
	DrawRect(Fill, X, Y, BarWidth * Fraction, BarHeight);

	// Hunger bar (H14): only the inhabited body carries survival pressure, so only draw it
	// while the player holds this animal — a spectated AI never starves. Stacked above the
	// stamina bar, sharing X and width; the fill lerps amber→red as the reservoir empties.
	if (bInhabited)
	{
		if (const UHungerComponent* Hunger = Animal->GetHunger())
		{
			const float HungerFraction = Hunger->GetHungerFraction();
			const float HungerY = Y - (HungerBarHeight + HungerBarGap);

			DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.55f),
				X - 3.f, HungerY - 3.f, BarWidth + 6.f, HungerBarHeight + 6.f);

			const FLinearColor HungerFill =
				FMath::Lerp(HungerEmptyColor, HungerFullColor, HungerFraction);
			DrawRect(HungerFill, X, HungerY, BarWidth * HungerFraction, HungerBarHeight);
		}
	}

	UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;
	if (!bInhabited)
	{
		DrawText(TEXT("AI has the body - press P to inhabit"), FLinearColor::White, X, Y - 24.f, Font);
	}
	else if (bExhausted)
	{
		DrawText(TEXT("Exhausted"), Fill, X, Y - 24.f, Font);
	}

	// Gait readout: the three-state locomotion model, legible at a glance.
	if (const ULocomotionComponent* Locomotion = Animal->GetLocomotion())
	{
		DrawText(GaitLabel(*Locomotion), FLinearColor(0.8f, 0.8f, 0.8f), X, Y + BarHeight + 8.f, Font);
	}

	// Species perception readout (debug switcher, SPECIES_PERCEPTION.md): which
	// senses are currently interpreting the world, and how to swap them.
	if (const USpeciesPerceptionComponent* Perception = Animal->GetPerception())
	{
		if (const USpeciesPerceptionProfile* Profile = Perception->GetActiveProfile())
		{
			const FString SpeciesLine = FString::Printf(TEXT("PERCEPTION: %s  (%d/%d — keys 1-4)"),
				*Profile->SpeciesName.ToString(),
				Perception->GetActiveProfileIndex() + 1,
				Perception->GetProfileCount());
			DrawText(SpeciesLine, FLinearColor(0.95f, 0.9f, 0.7f), 16.f, 16.f, Font);
		}
	}

	// Hunt readout (H14): the loop's whole state in a top-center block — the kill payoff
	// banner just after a catch, else the live run clock and a "closing in" catch meter.
	const UWorld* World = GetWorld();
	if (const UHuntSubsystem* Hunt = World ? World->GetSubsystem<UHuntSubsystem>() : nullptr)
	{
		const float CenterX = Canvas->SizeX * 0.5f;

		if (Hunt->GetSecondsSinceLastKill() < KillBannerSeconds)
		{
			// Feed beat landed: flash the outcome + how long that run took (the H14 metric).
			const FString Banner = FString::Printf(
				TEXT("DOWNED — fed   (hunt: %.1fs)"), Hunt->GetLastKillSeconds());
			DrawText(Banner, FLinearColor(0.4f, 0.9f, 0.45f),
				CenterX - HuntTextHalfWidth, HuntBlockTopMargin, Font);
		}
		else if (Hunt->IsHuntActive())
		{
			const FString Clock = FString::Printf(TEXT("HUNT  %.1fs"), Hunt->GetHuntElapsedSeconds());
			DrawText(Clock, FLinearColor(0.95f, 0.85f, 0.5f),
				CenterX - HuntTextHalfWidth, HuntBlockTopMargin, Font);

			// Closing-in meter: how far the hunter has held the catch window on any prey.
			const float Progress = Hunt->GetBestCatchProgress();
			if (Progress > 0.f)
			{
				const float BarX = CenterX - HuntBarWidth * 0.5f;
				const float BarY = HuntBlockTopMargin + HuntBarTopGap;
				DrawRect(FLinearColor(0.f, 0.f, 0.f, 0.55f),
					BarX - 2.f, BarY - 2.f, HuntBarWidth + 4.f, HuntBarHeight + 4.f);
				DrawRect(FLinearColor(0.9f, 0.35f, 0.2f), BarX, BarY, HuntBarWidth * Progress, HuntBarHeight);
			}
		}
	}

	// Spoor tracking cue (H14): once the quarry breaks close contact, the hunt continues by
	// reading the trail, not the animal (Liebenberg, The Art of Tracking). When the nearest
	// live prey is beyond reveal distance and a fresh trail sits within sniffing range, draw
	// an arrow toward its freshest point, coloured by how hot the trail still is. A trail that
	// ages out of range leaves no cue — the spoor has gone cold and the hunt is lost.
	if (bInhabited)
	{
		const UScentFieldSubsystem* Scent = World ? World->GetSubsystem<UScentFieldSubsystem>() : nullptr;
		if (Scent)
		{
			const FVector HunterLoc = Animal->GetActorLocation();

			// Nearest live (non-downed) prey — the "have I lost contact?" test.
			float NearestPreyDistSq = TNumericLimits<float>::Max();
			for (TActorIterator<AAnimalCharacter> It(World); It; ++It)
			{
				const AAnimalCharacter* Other = *It;
				if (!Other || Other == Animal || Other->IsDowned())
				{
					continue;
				}
				NearestPreyDistSq = FMath::Min(NearestPreyDistSq,
					static_cast<float>(FVector::DistSquared2D(HunterLoc, Other->GetActorLocation())));
			}

			FVector TrailDir = FVector::ZeroVector;
			float Freshness = 0.f;
			const bool bLostContact = NearestPreyDistSq > FMath::Square(TrackingRevealDistance);
			if (bLostContact &&
				Scent->GetFreshestTrailNear(HunterLoc, TrackingSniffRadius, TrailDir, Freshness))
			{
				// Project the world trail direction onto the fixed isometric screen using the
				// camera-yaw basis (the same frame WASD movement uses, D14): forward → up.
				const FRotator YawRot(0.f, Animal->GetCameraYaw(), 0.f);
				const FVector WorldForward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
				const FVector WorldRight = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
				FVector2D ScreenDir(
					FVector::DotProduct(TrailDir, WorldRight),
					-FVector::DotProduct(TrailDir, WorldForward));
				ScreenDir = ScreenDir.GetSafeNormal();

				const FVector2D Origin(Canvas->SizeX * 0.5f, Canvas->SizeY * SpoorScreenHeightFraction);
				const FVector2D Tip = Origin + ScreenDir * SpoorArrowLength;
				const FLinearColor SpoorColor = FMath::Lerp(SpoorColdColor, SpoorHotColor, Freshness);

				DrawLine(Origin.X, Origin.Y, Tip.X, Tip.Y, SpoorColor, 3.f);
				// Two arrowhead barbs swept back from the tip.
				const FVector2D Perp(-ScreenDir.Y, ScreenDir.X);
				const FVector2D Barb1 = (Perp - ScreenDir).GetSafeNormal();
				const FVector2D Barb2 = (-Perp - ScreenDir).GetSafeNormal();
				const FVector2D H1 = Tip + Barb1 * SpoorArrowHeadLength;
				const FVector2D H2 = Tip + Barb2 * SpoorArrowHeadLength;
				DrawLine(Tip.X, Tip.Y, H1.X, H1.Y, SpoorColor, 3.f);
				DrawLine(Tip.X, Tip.Y, H2.X, H2.Y, SpoorColor, 3.f);

				const TCHAR* Heat = Freshness > SpoorHotThreshold ? TEXT("HOT")
					: (Freshness > SpoorWarmThreshold ? TEXT("WARM") : TEXT("COLD"));
				DrawText(FString::Printf(TEXT("TRACKING · %s"), Heat), SpoorColor,
					Origin.X - HuntTextHalfWidth, Origin.Y + SpoorArrowLength + 6.f, Font);
			}
		}
	}
}
