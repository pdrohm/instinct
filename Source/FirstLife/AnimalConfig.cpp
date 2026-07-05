#include "AnimalConfig.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"

UAnimalConfig* UAnimalConfig::CreateHumanConfig(UObject* Outer)
{
	UAnimalConfig* Config = NewObject<UAnimalConfig>(Outer);

	Config->VisualStaticMesh = TSoftObjectPtr<UStaticMesh>(
		FSoftObjectPath(TEXT("/Game/Animals/Man/StaticMeshes/asian_old_man_warrior_with_cold_weapon.asian_old_man_warrior_with_cold_weapon")));
	Config->VisualMeshOffset = FVector(0.f, 0.f, -90.f);
	Config->VisualMeshRotation = FRotator(0.f, -90.f, 0.f);
	Config->VisualMeshScale = FVector(1.f, 1.f, 1.f);

	return Config;
}

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

	Config->VisualSkeletalMesh = TSoftObjectPtr<USkeletalMesh>(
		FSoftObjectPath(TEXT("/Game/Animals/Deer/Deer_001.Deer_001")));
	Config->VisualMeshOffset = FVector(0.f, 0.f, -72.f);
	Config->VisualMeshRotation = FRotator(0.f, -90.f, 0.f);
	Config->VisualMeshScale = FVector(1.f, 1.f, 1.f);

	// Speed-driven locomotion AnimBP (idle/walk/run BlendSpace on ground speed). Null-safe:
	// until this asset exists in the editor the mesh stays in bind pose; the moment you
	// author /Game/Animals/Deer/ABP_Deer it connects with no recompile. See docs/ANIMATION.md.
	Config->VisualAnimClass = TSoftClassPtr<UAnimInstance>(
		FSoftClassPath(TEXT("/Game/Animals/Deer/ABP_Deer.ABP_Deer_C")));

	// Built-in C++ locomotion clips (used until an ABP exists). Imported with the FBX.
	Config->IdleAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Deer/Deer_001_Anim_Deer_001_idle.Deer_001_Anim_Deer_001_idle")));
	Config->WalkAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Deer/Deer_001_Anim_Deer_001_walk.Deer_001_Anim_Deer_001_walk")));
	Config->RunAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Deer/Deer_001_Anim_Deer_001_run.Deer_001_Anim_Deer_001_run")));

	return Config;
}

UAnimalConfig* UAnimalConfig::CreateSaigaConfig(UObject* Outer)
{
	// All values FIRST-PASS GUESSES from a wildlife brief (Bekenov/Grachev/Milner-Gulland
	// saiga ecology) stated RELATIVE to the reindeer above, so the contrast is deliberate.
	UAnimalConfig* Config = NewObject<UAnimalConfig>(Outer);

	// Graze gait: faster leg cadence on a small frame, but recovers a touch slower on
	// the move than the caribou — a saiga is rarely fully relaxed. (first-pass)
	Config->Walk = FGaitSettings(200.f, 520.f, 5.f);

	// Escape cruise: out-cruises the reindeer (520) — built for flat-out open-plain
	// travel — but drains twice as fast (−4/s vs −2), a less efficient endurance gait.
	// (first-pass)
	Config->Jog = FGaitSettings(580.f, 760.f, -4.f);

	// Dash: above the reindeer dash (780) and far above the human sprint (620), but a
	// short fuse — ~24/s empties an 80 tank in ~3.5 s (vs the caribou's ~7 s). The saiga
	// is the faster animal you cannot simply outrun; you outlast its burst. (first-pass)
	Config->Sprint = FGaitSettings(900.f, 980.f, -24.f);

	// Same gait-mapping rule as the reindeer (SLICE2_CONTRACT): PreferredGait = Jog keeps
	// the escape-cruise reachable through the unchanged intent API. The saiga simply flees
	// to this cruise almost immediately and punches into Sprint under close threat.
	Config->PreferredGait = ELocomotionGait::Jog;

	// Nimbler and lighter than the reindeer (300/1200): jinks laterally to shake a
	// pursuer, and a light body decelerates harder for the cut. (first-pass)
	Config->TurnRateDegPerSecond = 380.f;
	Config->BrakingDeceleration = 1350.f;

	// The teeth of the design: a SHALLOW tank (80 vs 100) that REFILLS FAST when still
	// (24/s vs 18). Let a saiga stop and it resets — so a distance grind fails; the win
	// is relentless pressure that never lets it cash in that regen. (first-pass)
	Config->MaxStamina = 80.f;
	Config->RestRegenPerSecond = 24.f;
	Config->ExhaustionRecoveryFraction = 0.3f;

	// ~half a caribou's mass → a smaller meal: one saiga tops off less of the hunger bar
	// than a reindeer (85), so the saiga hunt must be repeated more often. (first-pass)
	Config->NutritionValue = 48.f;

	// Grey-box silhouette: SMALLER, PALER, twitchier — unmistakably not the dark bulky
	// reindeer at iso distance. ~0.8x reindeer linear scale (half mass, cube-root),
	// compact and slightly humped, head carried LOW with an over-large rounded snout
	// mass faking the saiga's signature down-turned proboscis.
	Config->CapsuleRadius = 40.f;
	Config->CapsuleHalfHeight = 56.f;
	Config->BodyOffset = FVector(0.f, 0.f, 4.f);
	Config->BodyScale = FVector(1.35f, 0.44f, 0.58f);
	Config->HeadOffset = FVector(78.f, 0.f, 26.f);
	Config->HeadScale = FVector(0.5f, 0.42f, 0.42f);

	// Pale sandy summer pelage: jumps off the dark terrain and contrasts hard with the
	// near-black reindeer — the two herds separate at a glance.
	Config->BodyColor = FLinearColor(0.62f, 0.55f, 0.40f);

	return Config;
}

UAnimalConfig* UAnimalConfig::CreateTigerConfig(UObject* Outer)
{
	UAnimalConfig* Config = NewObject<UAnimalConfig>(Outer);

	Config->Walk = FGaitSettings(180.f, 900.f, 2.f);
	Config->Jog = FGaitSettings(470.f, 1300.f, -3.f);
	Config->Sprint = FGaitSettings(980.f, 2600.f, -32.f);
	Config->PreferredGait = ELocomotionGait::Jog;

	Config->TurnRateDegPerSecond = 460.f;
	Config->BrakingDeceleration = 2000.f;
	Config->MaxStamina = 75.f;
	Config->RestRegenPerSecond = 16.f;
	Config->ExhaustionRecoveryFraction = 0.45f;
	Config->NutritionValue = 0.f;

	Config->CapsuleRadius = 52.f;
	Config->CapsuleHalfHeight = 62.f;
	Config->BodyOffset = FVector(0.f, 0.f, 2.f);
	Config->BodyScale = FVector(1.55f, 0.42f, 0.48f);
	Config->HeadOffset = FVector(92.f, 0.f, 34.f);
	Config->HeadScale = FVector(0.42f, 0.28f, 0.28f);
	Config->BodyColor = FLinearColor(0.55f, 0.28f, 0.08f);

	Config->VisualSkeletalMesh = TSoftObjectPtr<USkeletalMesh>(
		FSoftObjectPath(TEXT("/Game/Animals/Tiger/Tiger_001.Tiger_001")));
	Config->VisualMeshOffset = FVector(0.f, 0.f, -62.f);
	Config->VisualMeshRotation = FRotator(0.f, -90.f, 0.f);
	Config->VisualMeshScale = FVector(1.f, 1.f, 1.f);

	// Speed-driven locomotion AnimBP — null-safe until authored. See docs/ANIMATION.md.
	Config->VisualAnimClass = TSoftClassPtr<UAnimInstance>(
		FSoftClassPath(TEXT("/Game/Animals/Tiger/ABP_Tiger.ABP_Tiger_C")));

	// Built-in C++ locomotion clips (used until an ABP exists). Imported with the FBX.
	Config->IdleAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Tiger/Tiger_001_Anim_Tiger_001_idle.Tiger_001_Anim_Tiger_001_idle")));
	Config->WalkAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Tiger/Tiger_001_Anim_Tiger_001_walk.Tiger_001_Anim_Tiger_001_walk")));
	Config->RunAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Tiger/Tiger_001_Anim_Tiger_001_run.Tiger_001_Anim_Tiger_001_run")));

	return Config;
}

UAnimalConfig* UAnimalConfig::CreateWolfConfig(UObject* Outer)
{
	// First imported-animal visual hookup. The wolf uses the same generic pawn as
	// every other body; these numbers are only a starter profile so the mesh can
	// live in the simulation immediately.
	UAnimalConfig* Config = NewObject<UAnimalConfig>(Outer);

	Config->Walk = FGaitSettings(220.f, 700.f, 4.f);
	Config->Jog = FGaitSettings(560.f, 1100.f, -1.f);
	Config->Sprint = FGaitSettings(850.f, 1800.f, -18.f);
	Config->PreferredGait = ELocomotionGait::Jog;

	Config->TurnRateDegPerSecond = 520.f;
	Config->BrakingDeceleration = 1800.f;
	Config->MaxStamina = 95.f;
	Config->RestRegenPerSecond = 18.f;
	Config->ExhaustionRecoveryFraction = 0.35f;

	Config->NutritionValue = 0.f;

	// Capsule approximates the imported quadruped. The grey-box silhouette remains
	// filled as a fallback if the mesh is removed or not loaded.
	Config->CapsuleRadius = 38.f;
	Config->CapsuleHalfHeight = 58.f;
	Config->BodyOffset = FVector(0.f, 0.f, 0.f);
	Config->BodyScale = FVector(1.25f, 0.35f, 0.45f);
	Config->HeadOffset = FVector(72.f, 0.f, 28.f);
	Config->HeadScale = FVector(0.34f, 0.22f, 0.24f);
	Config->BodyColor = FLinearColor(0.12f, 0.12f, 0.12f);

	Config->VisualSkeletalMesh = TSoftObjectPtr<USkeletalMesh>(
		FSoftObjectPath(TEXT("/Game/Animals/Wolf/SkeletalMeshes/low_poly_wolf.low_poly_wolf")));
	Config->VisualMeshOffset = FVector(0.f, 0.f, -58.f);
	Config->VisualMeshRotation = FRotator(0.f, -90.f, 0.f);
	Config->VisualMeshScale = FVector(1.f, 1.f, 1.f);

	// Speed-driven locomotion AnimBP — null-safe until authored. Wolf clips live under
	// SkeletalMeshes/ (idle, running, sniffing). See docs/ANIMATION.md.
	Config->VisualAnimClass = TSoftClassPtr<UAnimInstance>(
		FSoftClassPath(TEXT("/Game/Animals/Wolf/ABP_Wolf.ABP_Wolf_C")));

	// Built-in C++ locomotion clips (used until an ABP exists). The wolf import has no
	// dedicated walk clip, so the run/lope covers both moving gaits for now.
	Config->IdleAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Wolf/SkeletalMeshes/idle.idle")));
	Config->WalkAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Wolf/SkeletalMeshes/running.running")));
	Config->RunAnim = TSoftObjectPtr<UAnimSequence>(
		FSoftObjectPath(TEXT("/Game/Animals/Wolf/SkeletalMeshes/running.running")));

	return Config;
}
