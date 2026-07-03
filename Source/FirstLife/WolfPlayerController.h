#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "WolfPlayerController.generated.h"

class AAnimalCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * The possession layer (H7): a player handing inputs to an ordinary animal agent —
 * currently the early human, previously the wolf; the seam does not care.
 * P toggles between inhabiting the agent and returning it to its own AI brain,
 * proving the seam the whole architecture rests on.
 *
 * Class keeps its original name to avoid churn ("do not restructure"); rename to
 * APossessionPlayerController is queued for a natural refactor window.
 *
 * All input objects are constructed in code so the slice needs zero binary assets.
 */
UCLASS()
class FIRSTLIFE_API AWolfPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

private:
	void BuildInputObjects();

	AAnimalCharacter* GetInhabitedAnimal() const;

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleSprintStarted(const FInputActionValue& Value);
	void HandleSprintCompleted(const FInputActionValue& Value);
	void HandleWalkStarted(const FInputActionValue& Value);
	void HandleWalkCompleted(const FInputActionValue& Value);
	void HandleTogglePossession(const FInputActionValue& Value);

	/** Debug species-perception switcher: keys 1-4 (SPECIES_PERCEPTION.md). */
	void HandleSelectSpecies(const FInputActionValue& Value, int32 ProfileIndex);

	/** Toggle the playtest telemetry overlay on the HUD (F1) — a feel-pass aid, not shipping UI. */
	void HandleToggleDebug(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY()
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY()
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY()
	TObjectPtr<UInputAction> ToggleAction;

	UPROPERTY()
	TObjectPtr<UInputAction> DebugAction;

	/** One action per species slot, mapped to keys 1-4. Debug switcher, not final UI. */
	UPROPERTY()
	TArray<TObjectPtr<UInputAction>> SpeciesActions;

	/** The agent we handed back to its own brain, so P can reclaim it. */
	TWeakObjectPtr<AAnimalCharacter> ReleasedAnimal;
};
