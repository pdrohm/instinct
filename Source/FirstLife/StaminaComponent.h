#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StaminaComponent.generated.h"

/**
 * The energy reservoir of one agent (H2): current charge, capacity, and the
 * exhaustion latch. Deliberately knows nothing about gaits or activities —
 * the locomotion system (and now hunger) decides the signed rate and pushes it
 * in through Update(). One battery, many consumers.
 *
 * Hunger couples in through SetHungerModifiers() (H14): two multipliers that scale
 * effective capacity and regen. The component never learns WHY they moved — it just
 * consumes them — so the starvation curve stays owned by UHungerComponent. Both
 * default to 1.0, so an animal under no hunger pressure (any AI body, the whole herd)
 * behaves exactly as before.
 *
 * Deliberately tickless: the owner drives Update() from its own tick so the
 * rate applied is never a frame stale. Works identically whether an AI or a
 * human drives the body (H7).
 */
UCLASS(ClassGroup = (FirstLife), meta = (BlueprintSpawnableComponent))
class FIRSTLIFE_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Set capacity and the exhaustion recovery threshold; refills to full. */
	void Configure(float InMaxStamina, float InExhaustionRecoveryFraction);

	/** Apply this frame's signed energy rate (units/s): regen positive, drain negative. */
	void Update(float RatePerSecond, float DeltaSeconds);

	/**
	 * The feed beat of the hunt (H14): running a prey down and eating it refills the
	 * hunter's reserve to full and clears the exhaustion latch. This is the payoff that
	 * closes the persistence loop — the energy you spent chasing comes back on the kill.
	 */
	void Refill();

	/**
	 * The hunger coupling seam (H14): scale effective capacity and regen. CapMult
	 * shrinks the usable reservoir; RegenMult throttles only positive rates. Pushed in
	 * each frame by the owner while the body is player-controlled; reset to (1, 1) the
	 * moment it isn't, so released/AI bodies are never starvation-penalized.
	 */
	void SetHungerModifiers(float InCapMult, float InRegenMult);

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStamina() const { return Current; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const { return Max; }

	/** Capacity actually reachable right now, after the hunger cap multiplier. */
	float GetEffectiveMaxStamina() const { return Max * HungerCapMult; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaFraction() const
	{
		const float EffectiveMax = GetEffectiveMaxStamina();
		return EffectiveMax > 0.f ? Current / EffectiveMax : 0.f;
	}

	/** True after a full burn-out, until stamina recovers past the recovery threshold. */
	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool IsExhausted() const { return bExhausted; }

	/** True when the agent has energy to spend on sprinting right now. */
	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool CanSprint() const { return !bExhausted && Current > 0.f; }

private:
	// Tuning, sourced from UAnimalConfig via Configure().
	float Max = 100.f;
	float ExhaustionRecoveryFraction = 0.3f;

	// Hunger coupling (H14). 1.0 = no pressure. Owned by UHungerComponent, pushed in via
	// SetHungerModifiers(); this component treats them as opaque scalars.
	float HungerCapMult = 1.f;
	float HungerRegenMult = 1.f;

	// State.
	float Current = 100.f;
	bool bExhausted = false;
};
