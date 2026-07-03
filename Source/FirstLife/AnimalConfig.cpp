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

	// Metabolism: the reindeer never gets hungry in play (only the player's body feels
	// hunger — see AAnimalCharacter::Tick), so MaxHunger/drain are inert defaults here.
	// What matters is NutritionValue: one caribou vastly exceeds one hunter's need, so a
	// kill restores MOST of the bar (+85, first-pass) — clamped at the gut, surplus lost.
	Config->NutritionValue = 85.f;

	// Grey-box quadruped: a long horizontal torso at ~110 cm shoulder height with
	// the head carried forward and high — at isometric distance the horizontal
	// silhouette vs the human's vertical one is the entire species read. Capsule
	// stays a vertical cylinder (CharacterMovement requirement); it wraps the
	// torso mass, not the head.
	Config->CapsuleRadius = 55.f;
	Config->CapsuleHalfHeight = 72.f;
	Config->BodyOffset = FVector(0.f, 0.f, 8.f);
	Config->BodyScale = FVector(1.7f, 0.5f, 0.62f);
	Config->HeadOffset = FVector(105.f, 0.f, 48.f);
	Config->HeadScale = FVector(0.42f, 0.22f, 0.3f);

	// Caribou pelage: grey-brown, clearly darker and cooler than the human's warm
	// ochre — hunter vs herd must separate at a glance before real meshes exist.
	Config->BodyColor = FLinearColor(0.16f, 0.13f, 0.10f);

	return Config;
}
