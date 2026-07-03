#include "StaminaComponent.h"

void UStaminaComponent::Configure(float InMaxStamina, float InExhaustionRecoveryFraction)
{
	Max = InMaxStamina;
	ExhaustionRecoveryFraction = InExhaustionRecoveryFraction;

	Current = Max;
	bExhausted = false;
}

void UStaminaComponent::SetHungerModifiers(float InCapMult, float InRegenMult)
{
	// Opaque scalars — the shape of the starvation curve lives in UHungerComponent.
	HungerCapMult = InCapMult;
	HungerRegenMult = InRegenMult;
}

void UStaminaComponent::Refill()
{
	// The feed beat (H14): a caught animal restores the hunter's reserve to full and
	// releases the exhaustion latch, mirroring Configure()'s fresh-body reset. Fills to
	// the CURRENTLY reachable cap — a starving hunter can't top off past its shrunken
	// ceiling, and the same-frame feed will have already begun lifting that ceiling.
	Current = GetEffectiveMaxStamina();
	bExhausted = false;
}

void UStaminaComponent::Update(float RatePerSecond, float DeltaSeconds)
{
	// Hunger throttles ONLY recovery — regen degrades when you starve, but drain does not
	// get any cheaper (starving doesn't make sprinting less costly). Negative rates pass through.
	if (RatePerSecond > 0.f)
	{
		RatePerSecond *= HungerRegenMult;
	}

	const float EffectiveMax = GetEffectiveMaxStamina();
	Current = FMath::Clamp(Current + RatePerSecond * DeltaSeconds, 0.f, EffectiveMax);

	if (!bExhausted && Current <= 0.f)
	{
		bExhausted = true;
	}
	else if (bExhausted && Current >= EffectiveMax * ExhaustionRecoveryFraction)
	{
		bExhausted = false;
	}
}
