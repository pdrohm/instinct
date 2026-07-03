#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HungerComponent.generated.h"

/**
 * The metabolic reservoir of one agent — the survival pressure that gives the hunt
 * a reason to exist (H14). A single scalar hunger[0..100] (100 = well-fed) that
 * drains on wall-clock and refills on a feed. Modelled on UStaminaComponent's
 * tickless-reservoir shape: the owner drives Update() from its own tick so the
 * drain is never a frame stale, and the same battery works whether an AI or a
 * human holds the body — though only the player's body actually feels the pressure
 * (see AAnimalCharacter::Tick).
 *
 * Deliberately NOT modelled (explicitly deferred — no hidden scope): hydration,
 * macronutrient / "rabbit starvation" balance, cold-calorie burn, and sleep. One
 * reservoir, one consequence curve. Depth comes later from systems interacting,
 * not from stacking meters now.
 *
 * Hunger also OWNS the shape of its own consequences (GetStamina*Mult): starvation
 * degrades performance in a specific order — recovery first and hardest (the death
 * spiral), capacity next — and that curve lives here, next to the value that drives
 * it, not smeared across the stamina system. The stamina component only consumes
 * the two multipliers; it never learns why they moved.
 */
UCLASS(ClassGroup = (FirstLife), meta = (BlueprintSpawnableComponent))
class FIRSTLIFE_API UHungerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Set capacity and drain rate from the species' UAnimalConfig; refills to full. */
	void Configure(float InMaxHunger, float InDrainPerSecond);

	/** Advance metabolism one frame: drain the reservoir on wall-clock, clamp to [0, Max]. */
	void Update(float DeltaSeconds);

	/**
	 * The feed beat of the hunt (H14): a kill restores hunger by the EATEN prey's
	 * NutritionValue. The clamp to Max IS the design — you cannot bank surplus in your
	 * gut, so one reindeer that far exceeds one hunter's need still only tops you off,
	 * and the overflow is lost. This is what keeps the hunt a recurring need, not a
	 * one-and-done stockpile.
	 */
	void Feed(float Amount);

	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetHunger() const { return Current; }

	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetMaxHunger() const { return Max; }

	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetHungerFraction() const { return Max > 0.f ? Current / Max : 0.f; }

	/**
	 * Piecewise-linear starvation penalties on stamina, pushed into UStaminaComponent
	 * each frame (the coupling seam). No penalty while well-fed; below the threshold each
	 * curve lerps down to a floor at empty. Capacity degrades from h<50 (floor 0.5);
	 * recovery degrades earlier and harder from h<60 (floor 0.3) — recovery is the
	 * death-spiral lever, so it is the first and steepest thing starvation takes.
	 */
	float GetStaminaCapMult() const;
	float GetStaminaRegenMult() const;

private:
	// Tuning, sourced from UAnimalConfig via Configure().
	float Max = 100.f;
	float DrainPerSecond = 0.083f;

	// State.
	float Current = 100.f;
};
