#pragma once

#include "CoreMinimal.h"
#include "HerdBrainTypes.h"

/**
 * One sensed herd-mate, as the brain sees it THIS frame. Deliberately a flat
 * snapshot, not a live actor pointer: cognition never reaches back into world truth.
 *
 * === U4 (Umwelt) SEAM ===
 * Today the controller fills this by raw proximity (TActorIterator). U4 will replace
 * that sensing step with a per-agent belief store — the brain will consume a
 * *belief* about a neighbor (last-known position, staleness, confidence) instead of
 * ground truth. FHerdBrain::Step already depends ONLY on this struct, so U4 swaps the
 * producer and leaves the cognition untouched.
 */
struct FBoidNeighbor
{
	FVector Location = FVector::ZeroVector;
	FVector Velocity = FVector::ZeroVector;
	/** The neighbor's current alarm scalar — the substrate for contagious flush. */
	float Alarm = 0.f;
};

/**
 * Everything the brain is allowed to know this frame. Proximity-sensed now; a belief
 * store later (U4). Threat = the nearest NON-herd animal (the player-possessed human).
 */
struct FHerdSense
{
	TArray<FBoidNeighbor> Neighbors;

	bool bHasThreat = false;
	FVector ThreatLocation = FVector::ZeroVector;
	FVector ThreatVelocity = FVector::ZeroVector;
};

/** The brain's output for one frame — expressed purely as intent (SLICE2_CONTRACT). */
struct FHerdDecision
{
	/** World-space steering direction (unit-ish); feed to AddMovementInput. */
	FVector MoveDirection = FVector::ZeroVector;
	/** AddMovementInput scale [0,1]; 0 during a graze pause. */
	float MoveScale = 0.f;
	/** SetWantsToWalk — true in Graze/Alert/Return (=> Walk 180). */
	bool bWantsToWalk = true;
	/** SetWantsToSprint — true only on FID breach / panic dash (=> Sprint 780). */
	bool bWantsToSprint = false;
};

/**
 * The prey herd brain (A1, validates H5). A plain per-agent struct — NOT a UObject —
 * owned by AAnimalAIController. It runs identically whether the body is AI- or
 * player-driven (D7) and degrades to solo graze/wander with zero herd-mates (H7).
 *
 * One boids core, two weight profiles (GRAZE/FLEE) blended by a single per-agent
 * alarm scalar [0,1], plus a predator-directed away vector and per-agent
 * heterogeneity (nervousness / reaction latency / boids-weight jitter) — the whole
 * substrate per research §3. The alarm scalar also carries the contagious-flush
 * signal neighbor-to-neighbor, so a straggler emerges without any global panic flag.
 *
 * Cognition only: it reads a FHerdSense snapshot and returns intent. It never touches
 * the pawn, the world, or velocity. The controller does sensing and applies intent.
 */
class FHerdBrain
{
public:
	/** Seed per-agent traits + home patch. Call once the body's location is known. */
	void Init(const FVector& HomeLocation);

	/** Advance one frame; returns the intent to apply. Pure function of its inputs. */
	FHerdDecision Step(float DeltaSeconds, const FHerdSense& Sense,
		const FVector& SelfLocation, const FVector& SelfVelocity, const FHerdBrainConfig& Config);

	/** Read-only alarm, so a neighbor's sensing step can propagate the flush. */
	float GetAlarm() const { return Alarm; }

	EHerdState GetState() const { return State; }

private:
	FVector ComputeBoidsSteering(const FHerdSense& Sense, const FVector& SelfLocation,
		const FVector& SelfVelocity, const FHerdBrainConfig& Config, float BlendAlarm,
		bool bCohesionBroken) const;

	float ComputeEffectiveFid(int32 HerdMateCount, const FHerdBrainConfig& Config,
		const FVector& SelfLocation, const FHerdSense& Sense) const;

	// ---- Per-agent immutable traits (heterogeneity — the straggler's origin) ----
	FVector HomeLocation = FVector::ZeroVector;
	/** >1 = jumpier than average, <1 = bolder. Scales alarm gain and FID. */
	float Nervousness = 1.f;
	/** Seconds this animal dithers before a personally-detected threat drives alarm. */
	float ReactionLatency = 0.f;
	/** Per-agent multiplicative jitter on each boids weight (cohesion/sep/align). */
	FVector BoidsJitter = FVector::OneVector;
	/** Slowly drifting graze heading, radians. */
	float WanderAngle = 0.f;
	bool bInitialized = false;

	// ---- Mutable state ----------------------------------------------------------
	EHerdState State = EHerdState::Graze;
	/** The one scalar that runs everything: [0,1]. */
	float Alarm = 0.f;
	/** Countdown of the reaction latency once a personal threat is present. */
	float ReactionTimer = 0.f;
	/** Permanent harassment memory — wary animals flush earlier afterward. */
	bool bHasBeenPressed = false;
	/** Graze pause bookkeeping. */
	bool bGrazePausing = false;
	float GrazePhaseTimer = 0.f;
};
