#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StaminaComponent.generated.h"

/**
 * The energy reservoir of one agent (H2): current charge, capacity, and the
 * exhaustion latch. Deliberately knows nothing about gaits or activities —
 * the locomotion system (and later heat, cold, hunger) decides the signed
 * rate and pushes it in through Update(). One battery, many consumers.
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

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStamina() const { return Current; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const { return Max; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaFraction() const { return Max > 0.f ? Current / Max : 0.f; }

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

	// State.
	float Current = 100.f;
	bool bExhausted = false;
};
