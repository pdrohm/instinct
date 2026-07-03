#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LocomotionTypes.h"

#include "AnimalConfig.generated.h"

/**
 * Data-driven body + energy definition for one species (H7): its locomotion
 * profile. A species is not "a character with different numbers" — it is a
 * different movement philosophy, and this asset is where that philosophy lives.
 * A new species later is a new instance of this asset, not a new class.
 * All values are editor-tunable so feel iteration never needs a recompile.
 *
 * Class defaults describe the early Homo sapiens: endurance over speed.
 * The jog regenerates — slowly — which is the species' defining trait (H2, H14):
 * humans are the animal that recovers *while moving*. Sprint is a tactical
 * purchase; jog is how you travel.
 *
 * Future species sketches (data only — see docs/LOCOMOTION.md):
 *   Wolf        — fast jog, StaminaDelta ≤ 0 while loping: must alternate run/recover.
 *   Big cat     — huge sprint speed + acceleration, massive drain, slow rest regen.
 *   Herbivore   — high jog speed, low acceleration, low turn rate, low braking (momentum).
 */
UCLASS(BlueprintType)
class FIRSTLIFE_API UAnimalConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Stalk/recover gait. For the human: strong regen on the move. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion|Gaits")
	FGaitSettings Walk = FGaitSettings(200.f, 768.f, 10.f);

	/**
	 * The cruising gait and the human default. Slightly regenerating (+4/s):
	 * sustainable indefinitely and slowly restorative — slower than walking,
	 * far slower than resting, but you keep covering ground. This asymmetry
	 * (recover while moving) is the persistence-hunting identity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion|Gaits")
	FGaitSettings Jog = FGaitSettings(400.f, 1200.f, 4.f);

	/**
	 * Burst gait: fast acceleration, heavy drain. Human default gives a ~8 s
	 * full burst (MaxStamina / 12) — always a tactical decision, never a habit.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion|Gaits")
	FGaitSettings Sprint = FGaitSettings(620.f, 2400.f, -12.f);

	/** The gait the body settles into when nothing is asked of it. AI travels at this. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion")
	ELocomotionGait PreferredGait = ELocomotionGait::Jog;

	/** How fast the body can re-orient, deg/s. Herbivore momentum = low value here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion", meta = (ClampMin = "0.0"))
	float TurnRateDegPerSecond = 540.f;

	/** Stopping power, cm/s². Low values make mass real: the body overshoots. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion", meta = (ClampMin = "0.0"))
	float BrakingDeceleration = 2048.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "1.0"))
	float MaxStamina = 100.f;

	/** Regen while (nearly) still — resting must beat every moving gait, clearly. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RestRegenPerSecond = 22.f;

	/**
	 * After burning out completely, sprint stays locked until stamina recovers to
	 * this fraction of max. This punishes greed and creates the movement rhythm.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ExhaustionRecoveryFraction = 0.3f;
};
