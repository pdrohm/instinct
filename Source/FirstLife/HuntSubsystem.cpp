#include "HuntSubsystem.h"

#include "AnimalAIController.h"
#include "AnimalCharacter.h"
#include "EngineUtils.h"
#include "FirstLife.h"
#include "GameFramework/PlayerController.h"
#include "StaminaComponent.h"

namespace
{
	// Reach of the catch, cm. The hunter must be this close to a blown prey to grab it.
	// First-pass guess: roughly one body-length of the grey-box human. Tune by feel.
	constexpr float CatchRadius = 200.f;
	// How long the hunter must HOLD the catch window before the prey goes down, seconds.
	// The honesty rule lives here — stepping out or letting the prey recover resets it.
	// First-pass guess: long enough that a glancing pass does not count, short enough
	// that a committed hunter is rewarded.
	constexpr float CatchDwellSeconds = 1.5f;
	// Herd alarm above which prey counts as "has noticed the hunter" and starts the hunt
	// clock. First-pass guess; the brain's alarm is [0..1] and contagion-driven.
	constexpr float HuntStartAlarmThreshold = 0.5f;
}

void UHuntSubsystem::Tick(float DeltaTime)
{
	// The kill banner fades on its own clock, independent of whether a hunt is running.
	if (SecondsSinceLastKill < TNumericLimits<float>::Max())
	{
		SecondsSinceLastKill += DeltaTime;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// HUNTER = the player's body (H7: player is just an inhabited animal). No player pawn
	// this frame → nothing to referee; drop any dwell so a re-possess starts clean.
	AAnimalCharacter* Hunter = nullptr;
	if (const APlayerController* PC = World->GetFirstPlayerController())
	{
		Hunter = Cast<AAnimalCharacter>(PC->GetPawn());
	}
	if (!Hunter)
	{
		ClearDwellState();
		return;
	}

	const FVector HunterLocation = Hunter->GetActorLocation();
	const float CatchRadiusSq = FMath::Square(CatchRadius);

	// Dwell is rebuilt fresh each tick: a prey that is NOT in the window this frame simply
	// never gets re-added, so its accumulated dwell vanishes — that reset IS the honesty
	// rule (interruptible catch). Carry-forward happens by reading last frame's value.
	TMap<TWeakObjectPtr<AAnimalCharacter>, float> NextDwell;
	bool bAnyLivePreyAlarmed = false;
	float BestProgress = 0.f;
	AAnimalCharacter* CaughtPrey = nullptr;

	// PREY = every other live animal. O(n^2)-ish across a tiny cast — fine, no spatial
	// hash (same call the herd brain already makes, per SLICE2 scope).
	for (TActorIterator<AAnimalCharacter> It(World); It; ++It)
	{
		AAnimalCharacter* Prey = *It;
		if (!Prey || Prey == Hunter || Prey->IsDowned())
		{
			continue;
		}

		// Clock trigger: a live prey whose alarm has crossed the threshold has noticed the
		// hunter — the run is on. Read alarm off the prey's own brain (contagion substrate).
		if (const AAnimalAIController* PreyBrain = Cast<AAnimalAIController>(Prey->GetController()))
		{
			if (PreyBrain->GetHerdAlarm() > HuntStartAlarmThreshold)
			{
				bAnyLivePreyAlarmed = true;
			}
		}

		// Catch window (the whole mechanism): exhausted — i.e. inside the takeable latch —
		// AND within reach right now. Both must hold this very frame or dwell resets.
		const UStaminaComponent* PreyStamina = Prey->GetStamina();
		const bool bInWindow = PreyStamina && PreyStamina->IsExhausted()
			&& FVector::DistSquared(HunterLocation, Prey->GetActorLocation()) <= CatchRadiusSq;

		float Dwell = 0.f;
		if (bInWindow)
		{
			const float* Prior = DwellByPrey.Find(Prey);
			Dwell = (Prior ? *Prior : 0.f) + DeltaTime;
			NextDwell.Add(Prey, Dwell);
		}

		BestProgress = FMath::Max(BestProgress, Dwell / CatchDwellSeconds);

		// First prey to fill the dwell this frame is the catch (one kill per frame is plenty).
		if (!CaughtPrey && Dwell >= CatchDwellSeconds)
		{
			CaughtPrey = Prey;
		}
	}

	DwellByPrey = MoveTemp(NextDwell);
	BestCatchProgress = FMath::Clamp(BestProgress, 0.f, 1.f);

	// Hunt clock: starts the first frame a hunter has alarmed live prey, then accrues every
	// frame until a kill resets it. This is the H14 "time to run one down" metric.
	if (!bHuntActive && bAnyLivePreyAlarmed)
	{
		bHuntActive = true;
		HuntElapsedSeconds = 0.f;
	}
	if (bHuntActive)
	{
		HuntElapsedSeconds += DeltaTime;
	}

	if (CaughtPrey)
	{
		// The catch resolves (H14): prey goes down (brain silences, body collapses), the
		// hunter feeds and its reserve refills, and the run's duration is banked. Then the
		// clock resets so the next chase is measured on its own.
		CaughtPrey->SetDowned(true);
		if (UStaminaComponent* HunterStamina = Hunter->GetStamina())
		{
			HunterStamina->Refill();
		}

		LastKillSeconds = HuntElapsedSeconds;
		SecondsSinceLastKill = 0.f;

		bHuntActive = false;
		HuntElapsedSeconds = 0.f;
		BestCatchProgress = 0.f;
		DwellByPrey.Remove(CaughtPrey);

		UE_LOG(LogFirstLife, Log,
			TEXT("HUNT: %s run down in %.1fs — hunter %s fed"),
			*CaughtPrey->GetName(), LastKillSeconds, *Hunter->GetName());
	}
}

void UHuntSubsystem::ClearDwellState()
{
	// No hunter to score against: forget partial catches and blank the closing-in meter.
	// The hunt clock is left as-is; a possession gap should not erase an in-flight run.
	DwellByPrey.Reset();
	BestCatchProgress = 0.f;
}

TStatId UHuntSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UHuntSubsystem, STATGROUP_Tickables);
}

bool UHuntSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	// Game/PIE only — never referee a catch in the editor preview world.
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
