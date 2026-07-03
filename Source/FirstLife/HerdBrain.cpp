#include "HerdBrain.h"

namespace
{
	FVector SafeDir(const FVector& V)
	{
		return V.IsNearlyZero() ? FVector::ZeroVector : V.GetSafeNormal();
	}
}

void FHerdBrain::Init(const FVector& InHomeLocation)
{
	HomeLocation = InHomeLocation;

	// Per-agent heterogeneity lives ENTIRELY in the brain (SLICE2_CONTRACT boundary):
	// nervousness, reaction latency, boids-weight jitter. NOT speed/stamina. This is
	// what makes one animal lag on the flush and become the winnable straggler.
	Nervousness = FMath::FRandRange(0.75f, 1.25f);
	ReactionLatency = FMath::FRandRange(0.f, 1.f); // scaled by config max in Step
	BoidsJitter = FVector(
		FMath::FRandRange(-1.f, 1.f),
		FMath::FRandRange(-1.f, 1.f),
		FMath::FRandRange(-1.f, 1.f));
	WanderAngle = FMath::FRandRange(0.f, 2.f * PI);
	GrazePhaseTimer = FMath::FRandRange(0.f, 3.f); // desync the herd's graze pauses

	State = EHerdState::Graze;
	Alarm = 0.f;
	ReactionTimer = 0.f;
	bHasBeenPressed = false;
	bGrazePausing = false;
	bInitialized = true;
}

float FHerdBrain::ComputeEffectiveFid(int32 HerdMateCount, const FHerdBrainConfig& Config,
	const FVector& SelfLocation, const FHerdSense& Sense) const
{
	// FID is a decision SURFACE, not a constant (research §2): base bolt distance
	// modulated by approach directness x speed x herd size x prior harassment.
	float Fid = Config.FidBase;

	// Approach directness x speed: a threat moving straight at me, fast, flushes me
	// from farther. A tangential/slow approach lets me hold — the player's read.
	if (Sense.bHasThreat)
	{
		const FVector ToSelf = SafeDir(SelfLocation - Sense.ThreatLocation);
		const FVector ThreatHeading = SafeDir(Sense.ThreatVelocity);
		const float Directness = FVector::DotProduct(ToSelf, ThreatHeading); // -1..1
		const float ApproachSpeed = Sense.ThreatVelocity.Size();

		// Oblique OR slow => shrink FID toward the oblique/slow multiplier.
		// directness<=0 (moving away/across) or near-zero speed => fully oblique.
		const float SpeedFactor = FMath::Clamp(ApproachSpeed / Config.ApproachSpeedNormalization, 0.f, 1.f);
		const float DirectFactor = FMath::Clamp(Directness, 0.f, 1.f);
		const float Aggression = DirectFactor * SpeedFactor; // 0 oblique/slow .. 1 direct/fast
		Fid *= FMath::Lerp(Config.FidObliqueSlowMultiplier, 1.f, Aggression);
	}

	// Many-eyes: larger groups flush at greater distance (detection dominates).
	const float GroupBonus = FMath::Min(HerdMateCount * Config.FidPerMateBonus, Config.FidGroupBonusCap);
	Fid *= (1.f + GroupBonus);

	// Prior harassment: wary animals flush earlier.
	if (bHasBeenPressed)
	{
		Fid *= Config.FidPressedMultiplier;
	}

	// Nervousness scales the whole surface: jumpy animals bolt sooner.
	Fid *= Nervousness;

	return Fid;
}

FVector FHerdBrain::ComputeBoidsSteering(const FHerdSense& Sense, const FVector& SelfLocation,
	const FVector& SelfVelocity, const FHerdBrainConfig& Config, float BlendAlarm,
	bool bCohesionBroken) const
{
	// One boids core, two weight profiles blended by alarm (research §3). GRAZE is
	// weakly-aligned meander; FLEE spikes alignment for the coordinated flush.
	const FBoidsWeights& G = Config.GrazeWeights;
	const FBoidsWeights& F = Config.FleeWeights;
	float WCohesion  = FMath::Lerp(G.Cohesion,  F.Cohesion,  BlendAlarm);
	float WSeparation = FMath::Lerp(G.Separation, F.Separation, BlendAlarm);
	float WAlignment = FMath::Lerp(G.Alignment, F.Alignment, BlendAlarm);

	// Per-agent boids-weight jitter (heterogeneity) — subtle, keeps the flock from
	// looking like clones without touching speed/stamina.
	const float J = Config.BoidsWeightJitter;
	WCohesion  = FMath::Max(0.f, WCohesion  * (1.f + BoidsJitter.X * J));
	WSeparation = FMath::Max(0.f, WSeparation * (1.f + BoidsJitter.Y * J));
	WAlignment = FMath::Max(0.f, WAlignment * (1.f + BoidsJitter.Z * J));

	// Cohesion is BREAKABLE (research §3.3): a panicked animal cut from the herd
	// drops cohesion and runs solo — the winnable straggler, the emotional core.
	if (bCohesionBroken)
	{
		WCohesion = 0.f;
	}

	FVector Separation = FVector::ZeroVector;
	FVector Centroid = FVector::ZeroVector;
	FVector AlignHeading = FVector::ZeroVector;
	const int32 Count = Sense.Neighbors.Num();

	for (const FBoidNeighbor& N : Sense.Neighbors)
	{
		const FVector Offset = SelfLocation - N.Location;
		const float Dist = Offset.Size();
		if (Dist > KINDA_SMALL_NUMBER)
		{
			// Separation falls off with distance — stronger the closer the crowding.
			Separation += Offset.GetSafeNormal() / Dist;
		}
		Centroid += N.Location;
		AlignHeading += N.Velocity;
	}

	FVector Steering = FVector::ZeroVector;

	// Zero-herd-mate degradation (H7): with no neighbors these three terms are simply
	// zero — no divide, no crash. The animal falls back to wander/home/predator only.
	if (Count > 0)
	{
		Centroid /= Count;
		const FVector Cohesion = SafeDir(Centroid - SelfLocation);
		const FVector Align = SafeDir(AlignHeading);
		Steering += Cohesion * WCohesion;
		Steering += SafeDir(Separation) * WSeparation;
		Steering += Align * WAlignment;
	}

	// Predator-directed away vector (research §3.1): pure boids has no predator term.
	// Weight scales with alarm so it only dominates during the flush.
	if (Sense.bHasThreat && BlendAlarm > KINDA_SMALL_NUMBER)
	{
		const FVector Away = SafeDir(SelfLocation - Sense.ThreatLocation);
		Steering += Away * (Config.PredatorFleeWeight * BlendAlarm);
	}

	// Graze-only terms: wander (the "alive and idle" drift) and a weak home leash,
	// both faded out by alarm so they never fight the flee.
	const float CalmWeight = 1.f - BlendAlarm;
	if (CalmWeight > KINDA_SMALL_NUMBER)
	{
		const FVector WanderDir(FMath::Cos(WanderAngle), FMath::Sin(WanderAngle), 0.f);
		Steering += WanderDir * (Config.WanderWeight * CalmWeight);

		const FVector ToHome = HomeLocation - SelfLocation;
		if (ToHome.Size() > Config.HomeLeashRadius)
		{
			Steering += SafeDir(ToHome) * (Config.HomeBiasWeight * CalmWeight);
		}
	}

	Steering.Z = 0.f; // planar steering; terrain following is the engine's job
	return Steering;
}

FHerdDecision FHerdBrain::Step(float DeltaSeconds, const FHerdSense& Sense,
	const FVector& SelfLocation, const FVector& SelfVelocity, const FHerdBrainConfig& Config)
{
	FHerdDecision Decision;
	if (!bInitialized || DeltaSeconds <= 0.f)
	{
		Decision.bWantsToWalk = true;
		return Decision;
	}

	const int32 HerdMateCount = Sense.Neighbors.Num();

	// Drift the graze wander heading (a slow random walk on the angle).
	WanderAngle += FMath::DegreesToRadians(Config.WanderTurnDegPerSecond) * DeltaSeconds
		* FMath::FRandRange(-1.f, 1.f);

	// ---- 1. Personal threat signal (proximity; U4 belief store later) ------------
	float PersonalSignal = 0.f;
	bool bThreatInsideFid = false;
	bool bThreatInsideInnerFid = false;
	if (Sense.bHasThreat)
	{
		const float ThreatDist = FVector::Dist(SelfLocation, Sense.ThreatLocation);

		// Many-eyes detection: effective radius grows mildly with herd size (Pulliam).
		const float DetectBonus =
			FMath::Min(HerdMateCount * Config.DetectionPerMateBonus, Config.DetectionBonusCap);
		const float EffDetection = Config.DetectionRadius * (1.f + DetectBonus);

		const float EffFid = ComputeEffectiveFid(HerdMateCount, Config, SelfLocation, Sense);
		bThreatInsideFid = ThreatDist < EffFid;
		bThreatInsideInnerFid = ThreatDist < EffFid * Config.DashInnerFidFraction;

		if (bThreatInsideFid)
		{
			// Inside the bolt threshold: full personal alarm (after reaction latency).
			PersonalSignal = 1.f;
		}
		else if (ThreatDist < EffDetection)
		{
			// Sensed but beyond FID: head-up alertness that scales as it approaches.
			const float T = 1.f - FMath::Clamp((ThreatDist - EffFid) / FMath::Max(EffDetection - EffFid, 1.f), 0.f, 1.f);
			PersonalSignal = FMath::Lerp(Config.CalmAlarmThreshold, Config.AlertSignalCeiling, T);
		}
	}

	// Reaction latency: a personal BOLT signal only starts driving alarm after this
	// animal's own dither elapses — staggered latencies + contagion = a real flush
	// wave, and the slow reactor becomes the straggler.
	const float MyLatency = ReactionLatency * Config.MaxReactionLatencySeconds;
	if (PersonalSignal >= 1.f)
	{
		ReactionTimer += DeltaSeconds;
		if (ReactionTimer < MyLatency)
		{
			PersonalSignal = FMath::Min(PersonalSignal, Config.AlertSignalCeiling); // still just alert until reacted
		}
	}
	else
	{
		ReactionTimer = 0.f;
	}
	PersonalSignal = FMath::Clamp(PersonalSignal * Nervousness, 0.f, 1.f);

	// ---- 2. Contagious flush: copy the most-alarmed neighbor (research §2) --------
	float NeighborMaxAlarm = 0.f;
	for (const FBoidNeighbor& N : Sense.Neighbors)
	{
		NeighborMaxAlarm = FMath::Max(NeighborMaxAlarm, N.Alarm);
	}
	const float ContagionTarget = NeighborMaxAlarm * Config.ContagionCoupling;

	// ---- 3. Drive the single alarm scalar ----------------------------------------
	const float AlarmTarget = FMath::Max(PersonalSignal, ContagionTarget);
	if (AlarmTarget > Alarm)
	{
		// Fast contagious/detection rise toward the target (time constant ~0.2-0.4 s).
		const float Rise = FMath::Clamp(DeltaSeconds / Config.AlarmPropagationSeconds, 0.f, 1.f);
		Alarm += (AlarmTarget - Alarm) * Rise;
	}
	else
	{
		// Slow panic cooldown -> return-to-graze over ~8-15 s of no pressure.
		Alarm = FMath::Max(AlarmTarget, Alarm - Config.AlarmDecayPerSecond * DeltaSeconds);
	}
	Alarm = FMath::Clamp(Alarm, 0.f, 1.f);

	// Harassment memory: once genuinely pressed, stays wary (earlier FID) for good.
	if (Alarm >= Config.PressedMemoryAlarm)
	{
		bHasBeenPressed = true;
	}

	// ---- 4. State band (soft; alarm is the real driver) --------------------------
	const bool bCalm = Alarm <= Config.CalmAlarmThreshold;
	const bool bFleeing = Alarm >= Config.FleeAlarmThreshold;
	if (bFleeing)
	{
		State = EHerdState::Flee;
	}
	else if (bCalm)
	{
		State = EHerdState::Graze;
	}
	else
	{
		// Between calm and flee: Alert if a threat is still sensed & pressure rising;
		// Return if it's a cooldown with no near threat.
		State = (Sense.bHasThreat && PersonalSignal > Config.CalmAlarmThreshold)
			? EHerdState::Alert : EHerdState::Return;
	}

	// ---- 5. Cohesion break: panicked AND cut from the herd => solo (§3.3) --------
	bool bCohesionBroken = false;
	if (Alarm >= Config.CohesionBreakAlarm && HerdMateCount > 0)
	{
		FVector Centroid = FVector::ZeroVector;
		for (const FBoidNeighbor& N : Sense.Neighbors) { Centroid += N.Location; }
		Centroid /= HerdMateCount;
		bCohesionBroken = FVector::Dist(SelfLocation, Centroid) > Config.CohesionBreakDistance;
	}

	// ---- 6. Steering + gait intent (SLICE2_CONTRACT mapping) ---------------------
	Decision.MoveDirection = ComputeBoidsSteering(
		Sense, SelfLocation, SelfVelocity, Config, Alarm, bCohesionBroken);

	if (State == EHerdState::Flee)
	{
		// Flee cruise = Jog (clear walk AND sprint). Dash = Sprint on inner-FID breach
		// or high panic. NEVER assert walk here or the herd would cruise at 180.
		const bool bDash = (Alarm >= Config.DashAlarmThreshold) || bThreatInsideInnerFid;
		Decision.bWantsToWalk = false;
		Decision.bWantsToSprint = bDash;
		Decision.MoveScale = Config.FleeMoveScale;
	}
	else
	{
		// Graze / Alert / Return => Walk (assert walk-intent, per contract, or a
		// grazing herd reads as perpetually fleeing at Jog).
		Decision.bWantsToWalk = true;
		Decision.bWantsToSprint = false;

		// Graze pauses: brief head-down stops so the herd doesn't glide. Alert/Return
		// keep moving (they're repositioning), so pauses are graze-only.
		if (State == EHerdState::Graze)
		{
			GrazePhaseTimer -= DeltaSeconds;
			if (GrazePhaseTimer <= 0.f)
			{
				bGrazePausing = !bGrazePausing;
				GrazePhaseTimer = bGrazePausing
					? Config.GrazePauseSeconds * FMath::FRandRange(0.5f, 1.5f)
					: Config.GrazeStepSeconds * FMath::FRandRange(0.5f, 1.5f);
			}
			Decision.MoveScale = bGrazePausing ? 0.f : Config.GrazeMoveScale;
		}
		else
		{
			// Alert tightens the herd (cohesion already up via alarm blend); Return
			// re-forms it. Both amble at a touch above graze to reposition.
			bGrazePausing = false;
			Decision.MoveScale = Config.GrazeMoveScale;
		}
	}

	// Guard: if steering collapsed to zero (e.g. isolated & calm mid-pause) don't feed
	// a zero-length direction to AddMovementInput — just report zero scale.
	if (Decision.MoveDirection.IsNearlyZero())
	{
		Decision.MoveScale = 0.f;
	}
	else
	{
		Decision.MoveDirection = Decision.MoveDirection.GetSafeNormal();
	}

	return Decision;
}
