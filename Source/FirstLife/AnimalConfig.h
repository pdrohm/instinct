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
	/**
	 * Builds the reindeer species as a fresh runtime instance — data-as-code, the
	 * headless analog of a DA_Reindeer.uasset (ADR-E4: a species is a data asset on
	 * the one pawn class, never a subclass). ALL reindeer tuning lives here; spawners
	 * (GameMode) hold only positions. Values are first-pass guesses from
	 * docs/research/HERD_AND_PREY.md — playtest-tune everything, none is canon.
	 *
	 * Energy philosophy (§4, Carrier/Liebenberg): every gait faster than the human's,
	 * no positive-recovery moving gait — the pressed animal that never stands still
	 * never regains its burst. That structural asymmetry IS the persistence hunt.
	 */
	static UAnimalConfig* CreateReindeerConfig(UObject* Outer);

	/**
	 * Builds the saiga antelope — the "anti-reindeer" second prey (ADR-E4: a data
	 * instance, not a subclass). Where the caribou is a heavy dark marathoner you
	 * outlast, the saiga is a small pale burst-sprinter you must TRAP: it dashes
	 * faster than the human, out-cruises the reindeer, and flushes from much farther
	 * out — but its tank is shallow and it recovers fast the instant it stops, so the
	 * win is tempo (cut one out, keep unbroken pressure), not distance. Two configs
	 * make two herds for free: the brain discovers herd-mates by GetConfig() identity,
	 * so a saiga never flocks with a reindeer. Values first-pass from a wildlife brief
	 * (Bekenov/Milner-Gulland saiga ecology) — playtest-tune, none is canon.
	 */
	static UAnimalConfig* CreateSaigaConfig(UObject* Outer);

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

	// -- Metabolism (H14) ------------------------------------------------------
	// A species' hunger is data, exactly like its locomotion (ADR-E4): the pawn class
	// knows nothing, this asset says everything. Drives UHungerComponent — the survival
	// pressure that gives the hunt a reason to exist. Class defaults describe the human.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Metabolism", meta = (ClampMin = "1.0"))
	float MaxHunger = 100.f;

	/**
	 * Hunger lost per second, wall-clock. First-pass 0.083/s empties a full bar in ~20 min
	 * — the honest ordering of a real ~48 h glycogen-buffer→decline window, compressed
	 * ~144× to a session-legible pace. Playtest-tune; not canon.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Metabolism", meta = (ClampMin = "0.0"))
	float HungerDrainPerSecond = 0.083f;

	/**
	 * How much hunger THIS animal restores to whoever eats it. Nutrition lives on the
	 * EATEN body, not the hunter: the feed amount a kill delivers is simply the prey's
	 * NutritionValue, so there is no hardcoded feed constant anywhere in the hunt code
	 * (data-driven, ADR-E4). Human = 0 — you do not eat yourself; the reindeer sets 85
	 * in CreateReindeerConfig, enough that one kill tops off most of a hunter's bar.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Metabolism", meta = (ClampMin = "0.0"))
	float NutritionValue = 0.f;

	// -- Grey-box body ---------------------------------------------------------
	// A species is also a silhouette (art direction: readability > realism). Until
	// real meshes arrive (GATE-C), each species describes its own grey-box body:
	// capsule volume plus two engine-cube boxes (body + head). Same rule as
	// locomotion — the pawn class knows nothing, the data asset says everything.
	// All in cm; cube scale of 1.0 = a 100 cm box. Defaults: upright ~180 cm human.

	/** Collision capsule radius, cm. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (ClampMin = "1.0"))
	float CapsuleRadius = 34.f;

	/** Collision capsule half-height, cm. Must be >= radius. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (ClampMin = "1.0"))
	float CapsuleHalfHeight = 90.f;

	/** Torso box center, relative to capsule center. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FVector BodyOffset = FVector(0.f, 0.f, -14.f);

	/** Torso box extents as engine-cube scale (X = facing direction). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FVector BodyScale = FVector(0.38f, 0.5f, 1.44f);

	/** Head box center, relative to capsule center. Forward of the body = quadruped read. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FVector HeadOffset = FVector(4.f, 0.f, 74.f);

	/** Head box extents as engine-cube scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FVector HeadScale = FVector(0.24f, 0.24f, 0.26f);

	/**
	 * Species tint, applied to both boxes. Color language, not decoration: at
	 * isometric distance the tint is what separates hunter from herd at a glance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FLinearColor BodyColor = FLinearColor(0.45f, 0.34f, 0.25f);
};
