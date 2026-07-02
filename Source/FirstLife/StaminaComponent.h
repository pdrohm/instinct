#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StaminaComponent.generated.h"

class UAnimalConfig;

/** What the body is doing right now, as reported by the owning agent. */
UENUM()
enum class EStaminaActivity : uint8
{
	Resting,
	Moving,
	Sprinting
};

/**
 * The energy economy of one agent (H2). Owns drain/regen/exhaustion math;
 * the owning character reports activity, this component answers "can you afford it?".
 * Works identically whether an AI or a human is driving the body (H7).
 *
 * Deliberately tickless: the owner calls Update() from its own Tick so the
 * activity used for drain/regen is never a frame stale.
 */
UCLASS(ClassGroup = (FirstLife), meta = (BlueprintSpawnableComponent))
class FIRSTLIFE_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Pull tuning values from the species config. Call once the config is resolved. */
	void Configure(const UAnimalConfig& Config);

	/** Advance the energy math for this frame, given what the body is doing. */
	void Update(EStaminaActivity Activity, float DeltaSeconds);

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
	float SprintDrainPerSecond = 20.f;
	float WalkRegenPerSecond = 8.f;
	float RestRegenPerSecond = 25.f;
	float ExhaustionRecoveryFraction = 0.3f;

	// State.
	float Current = 100.f;
	bool bExhausted = false;
};
