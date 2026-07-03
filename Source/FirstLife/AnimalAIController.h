#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HerdBrainTypes.h"
#include "HerdBrain.h"

#include "AnimalAIController.generated.h"

class AAnimalCharacter;

/**
 * The animal's own brain (A1, validates H5). Possession is a real seam (H7):
 * releasing the player hands the same body back to this AI, and the herd brain runs
 * identically whether AI- or player-driven (D7) — a controller only expresses intent.
 *
 * This controller is deliberately thin. It does two things per tick:
 *   1. SENSE — gather herd-mates (same GetConfig() pointer, within neighbor radius)
 *      and the nearest non-herd threat, by proximity. [U4 replaces this with a belief
 *      store; see the FHerdSense seam comment in HerdBrain.h.]
 *   2. APPLY — hand the sense to FHerdBrain (all cognition) and translate its intent
 *      into the pawn's intent API (AddMovementInput / SetWantsToWalk / SetWantsToSprint).
 *
 * All behavior + tuning lives in FHerdBrain / FHerdBrainConfig, not here.
 */
UCLASS()
class FIRSTLIFE_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAnimalAIController();

	virtual void Tick(float DeltaTime) override;

	/** Exposed so a neighbor's sensing step can read this agent's alarm (contagion). */
	float GetHerdAlarm() const { return Brain.GetAlarm(); }

protected:
	virtual void OnPossess(APawn* InPawn) override;

	/** All herd-brain tuning — one editor-tunable block. Every value is a first-pass guess. */
	UPROPERTY(EditAnywhere, Category = "Herd Brain")
	FHerdBrainConfig BrainConfig;

private:
	/**
	 * Proximity sensing (U4 Umwelt seam): fill a FHerdSense from world truth today.
	 * Herd-mate = another AAnimalCharacter whose GetConfig() pointer-equals ours,
	 * within NeighborRadius. Threat = nearest non-herd AAnimalCharacter.
	 */
	FHerdSense SenseHerd(const AAnimalCharacter& Self) const;

	FHerdBrain Brain;
};
