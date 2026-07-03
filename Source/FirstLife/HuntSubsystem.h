#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "HuntSubsystem.generated.h"

class AAnimalCharacter;

/**
 * The persistence-hunt core loop (H14 — the make-or-break slice). This subsystem is the
 * referee of the catch: it watches the one player-driven hunter and every live prey, and
 * turns the stamina economy into a kill.
 *
 * The whole mechanic rests on one wildlife-research finding: the exhaustion latch IS the
 * catch window. A blown, hyperthermic ungulate that has just stopped is briefly takeable,
 * but it recovers its escape ability once it cools back to ~30% of stamina. So
 * Stamina->IsExhausted() is exactly "this animal is in the takeable window" — there is no
 * separate downable state, and downing is simply the *result* of a successful catch.
 *
 * Two things are measured/enforced per tick:
 *   - HUNT CLOCK: time-to-run-one-down, the H14 metric. Starts when a hunter first
 *     alarms live prey, accumulates until a kill, then resets.
 *   - CATCH DWELL (honesty rule): the hunter must STAY in the catch window. Dwell only
 *     accrues while a prey is both exhausted and inside CatchRadius; stepping away — or
 *     letting the prey recover past 30% — resets it and forces a re-chase.
 *
 * FAKE, on purpose (like UScentFieldSubsystem): no eating animation, no meat/energy
 * accounting beyond a full refill, no downed-body persistence. This exists only to test
 * whether "run it down until it drops, then feed" is a loop worth building on.
 */
UCLASS()
class FIRSTLIFE_API UHuntSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	/** True while a hunt clock is running (hunter exists and has alarmed live prey). */
	UFUNCTION(BlueprintPure, Category = "Hunt")
	bool IsHuntActive() const { return bHuntActive; }

	/** Seconds the current hunt has been running — the live time-to-run-down readout. */
	UFUNCTION(BlueprintPure, Category = "Hunt")
	float GetHuntElapsedSeconds() const { return HuntElapsedSeconds; }

	/** Duration of the most recent successful catch, or a negative sentinel if none yet. */
	UFUNCTION(BlueprintPure, Category = "Hunt")
	float GetLastKillSeconds() const { return LastKillSeconds; }

	/** 0..1 "closing in" meter: the best (largest) catch dwell across prey this frame. */
	UFUNCTION(BlueprintPure, Category = "Hunt")
	float GetBestCatchProgress() const { return BestCatchProgress; }

	/** Time since the last kill landed, so the HUD can flash a banner then hide it. */
	UFUNCTION(BlueprintPure, Category = "Hunt")
	float GetSecondsSinceLastKill() const { return SecondsSinceLastKill; }

private:
	/** Resets the per-frame dwell bookkeeping when there is no active hunt to score. */
	void ClearDwellState();

	// --- State (all owned here; the HUD reads it, nothing writes it) ---------
	bool bHuntActive = false;
	float HuntElapsedSeconds = 0.f;

	/** Negative until the first kill — the HUD treats < 0 as "no hunt completed yet". */
	float LastKillSeconds = -1.f;

	/** Large so the banner is hidden at startup; only a real kill drives it to 0. */
	float SecondsSinceLastKill = TNumericLimits<float>::Max();

	float BestCatchProgress = 0.f;

	/** How long the hunter has held each prey inside the catch window (interruptible). */
	TMap<TWeakObjectPtr<AAnimalCharacter>, float> DwellByPrey;
};
