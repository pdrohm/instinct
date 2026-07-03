#include "HungerComponent.h"

namespace
{
	// --- Starvation consequence curves (H14) — all FIRST-PASS GUESSES from the
	// physiology pass, tune by feel. Piecewise-linear with a floor: no penalty while
	// well-fed, then a straight lerp down to the floor at empty. ---
	//
	// Capacity: the reservoir you can hold shrinks as you starve, but slower and later
	// than recovery — a starving animal can still muster a burst, it just can't hold or
	// refill much of one.
	constexpr float CapWellFedThreshold = 50.f; // At/above this hunger, full capacity.
	constexpr float CapFloorMult = 0.5f;         // Capacity multiplier at hunger 0.
	//
	// Recovery: degrades FIRST (from a higher hunger) and HARDEST (to a lower floor).
	// This is the death spiral — the starving animal can't recover its burst on the
	// move, so every chase costs more than the last. This asymmetry is the whole point.
	constexpr float RegenWellFedThreshold = 60.f; // At/above this hunger, full regen.
	constexpr float RegenFloorMult = 0.3f;        // Regen multiplier at hunger 0.
}

void UHungerComponent::Configure(float InMaxHunger, float InDrainPerSecond)
{
	Max = InMaxHunger;
	DrainPerSecond = InDrainPerSecond;

	Current = Max;
}

void UHungerComponent::Feed(float Amount)
{
	// The clamp IS the "can't bank surplus in your gut" cap (H14): overflow is lost.
	Current = FMath::Min(Max, Current + Amount);
}

void UHungerComponent::Update(float DeltaSeconds)
{
	// Metabolism only ever runs down here — the sole way back up is Feed() on a kill.
	Current = FMath::Clamp(Current - DrainPerSecond * DeltaSeconds, 0.f, Max);
}

float UHungerComponent::GetStaminaCapMult() const
{
	if (Current >= CapWellFedThreshold)
	{
		return 1.f;
	}
	// Below the threshold: how far into the starving band are we (0 at threshold, 1 at empty)?
	const float StarveAlpha = (CapWellFedThreshold - Current) / CapWellFedThreshold;
	return FMath::Lerp(1.f, CapFloorMult, FMath::Clamp(StarveAlpha, 0.f, 1.f));
}

float UHungerComponent::GetStaminaRegenMult() const
{
	if (Current >= RegenWellFedThreshold)
	{
		return 1.f;
	}
	const float StarveAlpha = (RegenWellFedThreshold - Current) / RegenWellFedThreshold;
	return FMath::Lerp(1.f, RegenFloorMult, FMath::Clamp(StarveAlpha, 0.f, 1.f));
}
