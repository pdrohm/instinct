#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AnimalConfig.generated.h"

/**
 * Data-driven body + energy definition for one species (H7).
 * A second species later should be a new instance of this asset, not a new class.
 * All values are editor-tunable so chase/energy feel iteration never needs a recompile.
 */
UCLASS(BlueprintType)
class FIRSTLIFE_API UAnimalConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Sustainable ground speed, cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 300.f;

	/** Burst speed while sprinting, cm/s. Sprinting is the whole action budget (H2). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "1.0"))
	float MaxStamina = 100.f;

	/** Drain while actually sprint-moving. Default gives a ~5 second full burst. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float SprintDrainPerSecond = 20.f;

	/** Regen while moving at walk speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float WalkRegenPerSecond = 8.f;

	/** Regen while (nearly) still — resting must feel meaningfully better than walking. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RestRegenPerSecond = 25.f;

	/**
	 * After burning out completely, sprint stays locked until stamina recovers to this
	 * fraction of max. This is what punishes greed and creates the movement rhythm.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ExhaustionRecoveryFraction = 0.3f;
};
