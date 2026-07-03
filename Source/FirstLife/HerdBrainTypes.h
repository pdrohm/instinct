#pragma once

#include "CoreMinimal.h"

#include "HerdBrainTypes.generated.h"

/**
 * The prey herd's emotional/behavioral state (A1, validates H5). This is NOT a
 * locomotion gait (that lives in ULocomotionComponent) — it is what the brain is
 * *trying to do*. Gait is a downstream consequence expressed through the intent API
 * per SLICE2_CONTRACT.md's mapping table.
 *
 * The four states are a soft banding of a single continuous alarm scalar [0,1], not
 * a hard FSM: alarm is what actually drives the boids weight blend and the gait
 * intent. The enum exists for readability, debug, and the two places where a state
 * genuinely branches behavior (Alert tightens cohesion; Flee flips the gait).
 */
UENUM(BlueprintType)
enum class EHerdState : uint8
{
	/** Weak-alignment boids + slow wander + weak home bias. Walk gait, graze pauses. */
	Graze,
	/** Head-up, cohesion tightens, still walking — threat sensed but under FID. */
	Alert,
	/** Alignment spikes + predator-away vector. Jog cruise, Sprint dash on FID breach. */
	Flee,
	/** Panic decaying, threat gone — re-asserting walk on the way back to Graze. */
	Return
};

/**
 * One Reynolds boids weight profile. Two of these (GRAZE / FLEE) are blended by the
 * per-agent alarm scalar to produce the whole behavioral range from one core
 * (research §3: "one boids core, two weight profiles blended by a per-agent alarm").
 */
USTRUCT(BlueprintType)
struct FBoidsWeights
{
	GENERATED_BODY()

	FBoidsWeights() = default;
	FBoidsWeights(float InCohesion, float InSeparation, float InAlignment)
		: Cohesion(InCohesion), Separation(InSeparation), Alignment(InAlignment) {}

	/** Steer toward local herd centroid — the "stay together" pull. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Cohesion = 0.6f;

	/** Avoid crowding nearest neighbors — personal space. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Separation = 1.2f;

	/** Match neighbors' heading — low when grazing, spikes on flush (coordinated flee). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Alignment = 0.3f;
};

/**
 * ALL herd-brain tuning, centralized as one editor-tunable block on the controller.
 *
 * EVERY value here is a FIRST-PASS GUESS from docs/research/HERD_AND_PREY.md §"Tuning
 * table" — none is canon, all expect to move in playtest. Grouped so a designer can
 * find "the flee feel" or "the FID surface" without reading the brain code.
 */
USTRUCT(BlueprintType)
struct FHerdBrainConfig
{
	GENERATED_BODY()

	// ---- Boids substrate --------------------------------------------------------

	/** Who counts as a herd-mate / neighbor for boids, cm. (first-pass: 800) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float NeighborRadius = 800.f;

	/** GRAZE profile: separation > cohesion, weak alignment = idle meander. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids")
	FBoidsWeights GrazeWeights = FBoidsWeights(0.6f, 1.2f, 0.3f);

	/** FLEE profile: alignment spikes = coordinated flush; slight de-separation = bunch. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids")
	FBoidsWeights FleeWeights = FBoidsWeights(0.8f, 0.9f, 1.8f);

	/** The "alive and idle" random-heading drift, graze only. (first-pass: 0.4) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float WanderWeight = 0.4f;

	/** How fast the graze wander heading drifts, deg/s. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float WanderTurnDegPerSecond = 40.f;

	/** Weak pull back toward the home graze patch when beyond leash. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float HomeBiasWeight = 0.35f;

	/** Distance from home before the home bias kicks in, cm. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float HomeLeashRadius = 1500.f;

	/** The predator-directed away vector's flee weight (research §3.1). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids", meta = (ClampMin = "0.0"))
	float PredatorFleeWeight = 2.5f;

	// ---- Perception (proximity only this slice; U4 Umwelt replaces this) --------

	/** Base line-of-sight detection radius, cm. No scent yet (Stage 2). (first-pass: 2500) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (ClampMin = "0.0"))
	float DetectionRadius = 2500.f;

	/** Many-eyes: detection grows this fraction per herd-mate in range (Pulliam). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (ClampMin = "0.0"))
	float DetectionPerMateBonus = 0.05f;

	/** Cap on the many-eyes detection bonus. (first-pass: +40%) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (ClampMin = "0.0"))
	float DetectionBonusCap = 0.40f;

	// ---- FID as a decision surface (research §2, NOT a constant) -----------------

	/** Base bolt distance for a direct, fast approach, cm. Modulated below. (first-pass: 1400) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0"))
	float FidBase = 1400.f;

	/** Multiplier when approach is oblique/slow — rewards the player's read. (first-pass: 0.5) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0"))
	float FidObliqueSlowMultiplier = 0.5f;

	/** Multiplier once this animal has been pressed before — wary flush earlier. (first-pass: 1.4) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0"))
	float FidPressedMultiplier = 1.4f;

	/** FID grows this fraction per herd-mate in range (larger groups flush sooner). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0"))
	float FidPerMateBonus = 0.04f;

	/** Cap on the herd-size FID bonus. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0"))
	float FidGroupBonusCap = 0.5f;

	/** Inner FID fraction: threat this close forces the Sprint dash, not the Jog cruise. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DashInnerFidFraction = 0.5f;

	// ---- Alarm dynamics (the one scalar that carries everything) -----------------

	/** Alarm >= this => Flee band (gait flips to Jog/Sprint). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float FleeAlarmThreshold = 0.45f;

	/** Alarm >= this => the panic-dash (Sprint) even without an inner-FID breach. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DashAlarmThreshold = 0.8f;

	/**
	 * Alarm <= this => fully calm again (Return -> Graze). Also the alert-band floor:
	 * a sensed threat below this alarm reads as calm/grazing. (first-pass)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CalmAlarmThreshold = 0.15f;

	/** A sensed-but-distant threat drives alarm no higher than this (head-up, not bolt). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AlertSignalCeiling = 0.4f;

	/** Approach speed (cm/s) treated as fully "fast" when scaling FID by aggression (~human jog). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FID", meta = (ClampMin = "1.0"))
	float ApproachSpeedNormalization = 400.f;

	/** Contagion time constant, s: how fast alarm rises toward an alarmed neighbor. (first-pass: 0.2-0.4) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.01"))
	float AlarmPropagationSeconds = 0.3f;

	/** Panic cooldown rate per second with no pressure -> ~8-15 s to graze. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0"))
	float AlarmDecayPerSecond = 0.09f;

	/** Contagion only copies a neighbor's alarm scaled by this (never quite matches). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ContagionCoupling = 0.9f;

	/** Alarm level that stamps the permanent "has been pressed" harassment memory. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Alarm", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PressedMemoryAlarm = 0.6f;

	// ---- Cohesion break / the winnable straggler (research §3.3) -----------------

	/** Panicked AND farther than this from the herd centroid => drop cohesion, run solo, cm. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Split", meta = (ClampMin = "0.0"))
	float CohesionBreakDistance = 1500.f;

	/** Alarm above which cohesion can break. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Split", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CohesionBreakAlarm = 0.7f;

	// ---- Movement input scales (graze ambles, flee commits) ----------------------

	/** AddMovementInput scale while grazing — the herd ambles, it does not march. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GrazeMoveScale = 0.35f;

	/** AddMovementInput scale while fleeing — full commitment. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float FleeMoveScale = 1.0f;

	/** Mean seconds of grazing between brief head-down pauses. (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1"))
	float GrazeStepSeconds = 4.f;

	/** Mean seconds a graze pause lasts (movement scale ~0). (first-pass) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.1"))
	float GrazePauseSeconds = 1.5f;

	// ---- Per-agent heterogeneity IN THE BRAIN (makes the straggler emerge) -------
	// NOTE: per-agent max-speed/stamina variation is deliberately NOT here — that
	// would need a UAnimalConfig/seam change (SLICE2_CONTRACT heterogeneity boundary).
	// Reaction/nervousness/weight jitter is enough for the emergent straggler.

	/** +/- fractional jitter on nervousness (alarm gain & FID). (first-pass: 0.25) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heterogeneity", meta = (ClampMin = "0.0"))
	float NervousnessJitter = 0.25f;

	/** Max per-agent reaction latency before a personally-detected threat drives alarm, s. (first-pass: 0.3) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heterogeneity", meta = (ClampMin = "0.0"))
	float MaxReactionLatencySeconds = 0.3f;

	/** +/- fractional jitter applied per-agent to each boids weight. (first-pass: 0.3) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heterogeneity", meta = (ClampMin = "0.0"))
	float BoidsWeightJitter = 0.3f;
};
