#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AnimalConfig.generated.h"

/**
 * Data-driven body + energy definition for one species (H7).
 * A second species later should be a new instance of this asset, not a new class.
 * All values are editor-tunable so chase/energy feel iteration never needs a recompile.
 *
 * Class defaults describe the early Homo sapiens (2026-07-03 pivot): a poor sprinter
 * with a sustainable run — endurance is the species' whole strategy (H2, H14).
 */
UCLASS(BlueprintType)
class FIRSTLIFE_API UAnimalConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Slow, deliberate pace, cm/s. Recovers stamina while moving — the stalk/recover gait. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 200.f;

	/**
	 * Sustainable endurance run, cm/s — the default gait. Costs nothing, regains nothing
	 * (see RunRegenPerSecond). For the human this is the persistence-hunting pace: slower
	 * than any prey's sprint, faster than their recovery walk.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float RunSpeed = 400.f;

	/** Burst speed while sprinting, cm/s. Sprinting is the whole action budget (H2). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 620.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "1.0"))
	float MaxStamina = 100.f;

	/** Drain while actually sprint-moving. Default gives a ~8 second full burst. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float SprintDrainPerSecond = 12.f;

	/**
	 * Regen while running. 0 = the run is exactly sustainable, which is the human's
	 * biological identity; a positive value would make running strictly free.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RunRegenPerSecond = 0.f;

	/** Regen while moving at walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float WalkRegenPerSecond = 10.f;

	/** Regen while (nearly) still — resting must feel meaningfully better than walking. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RestRegenPerSecond = 22.f;

	/**
	 * After burning out completely, sprint stays locked until stamina recovers to this
	 * fraction of max. This is what punishes greed and creates the movement rhythm.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ExhaustionRecoveryFraction = 0.3f;
};
