#include "AnimalConfig.h"

UAnimalConfig* UAnimalConfig::CreateReindeerConfig(UObject* Outer)
{
	// All values FIRST-PASS GUESSES from docs/research/HERD_AND_PREY.md tuning
	// table — starting points for playtest iteration, not design commitments.
	UAnimalConfig* Config = NewObject<UAnimalConfig>(Outer);

	// Graze gait: slow, mildly restorative — but real recovery only pays out
	// near-still (RestRegen). Low acceleration = herbivore momentum. (first-pass)
	Config->Walk = FGaitSettings(180.f, 500.f, 6.f);

	// Escape cruise: faster than the human jog (400) and slightly draining (−2/s)
	// — sustainable-ish flee, but unlike the human it never recovers on the move.
	// (first-pass)
	Config->Jog = FGaitSettings(520.f, 700.f, -2.f);

	// Dash: above human sprint (620) — the dash is unwinnable for the hunter.
	// Heavy drain empties 100 stamina in ~7 s (folklore-corrected gallop, low end).
	// (first-pass)
	Config->Sprint = FGaitSettings(780.f, 900.f, -16.f);

	// Jog, NOT Walk (research table guesses Walk) — SLICE2_CONTRACT.md gait mapping:
	// ResolveGait() reaches only { sprint-intent → Sprint, walk-intent → Walk,
	// else → PreferredGait } and there is no SetWantsToJog. PreferredGait = Jog keeps
	// all three reindeer gaits reachable through the unchanged intent API; the herd
	// brain asserts SetWantsToWalk(true) as its graze default.
	Config->PreferredGait = ELocomotionGait::Jog;

	// More maneuverable than the human (540): cornering is the prey's defense —
	// but heavy: braking well below human (2048), the body overshoots. (first-pass)
	Config->TurnRateDegPerSecond = 300.f;
	Config->BrakingDeceleration = 1200.f;

	// Same stamina scale as the human for readability. Rest regen is the crux:
	// it only pays out near-still, so a pressed animal never regains its burst.
	// (first-pass)
	Config->MaxStamina = 100.f;
	Config->RestRegenPerSecond = 18.f;
	Config->ExhaustionRecoveryFraction = 0.3f;

	return Config;
}
