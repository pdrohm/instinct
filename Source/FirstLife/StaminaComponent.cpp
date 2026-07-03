#include "StaminaComponent.h"

void UStaminaComponent::Configure(float InMaxStamina, float InExhaustionRecoveryFraction)
{
	Max = InMaxStamina;
	ExhaustionRecoveryFraction = InExhaustionRecoveryFraction;

	Current = Max;
	bExhausted = false;
}

void UStaminaComponent::Refill()
{
	// The feed beat (H14): a caught animal restores the hunter's reserve to full and
	// releases the exhaustion latch, mirroring Configure()'s fresh-body reset.
	Current = Max;
	bExhausted = false;
}

void UStaminaComponent::Update(float RatePerSecond, float DeltaSeconds)
{
	Current = FMath::Clamp(Current + RatePerSecond * DeltaSeconds, 0.f, Max);

	if (!bExhausted && Current <= 0.f)
	{
		bExhausted = true;
	}
	else if (bExhausted && Current >= Max * ExhaustionRecoveryFraction)
	{
		bExhausted = false;
	}
}
