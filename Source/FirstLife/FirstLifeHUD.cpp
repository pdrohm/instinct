#include "FirstLifeHUD.h"

#include "AnimalCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "StaminaComponent.h"

namespace
{
	constexpr float BarWidth = 420.f;
	constexpr float BarHeight = 16.f;
	constexpr float BarBottomMargin = 64.f;
	constexpr float LowStaminaFraction = 0.3f;
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
}
