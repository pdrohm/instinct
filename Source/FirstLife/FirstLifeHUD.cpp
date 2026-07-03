#include "FirstLifeHUD.h"

#include "AnimalCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "HuntSubsystem.h"
#include "LocomotionComponent.h"
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
}
