#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "WolfPlayerController.generated.h"

class AAnimalCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * The possession layer (H7): a human handing inputs to an ordinary animal agent.
 * P toggles between inhabiting the wolf and returning it to its own AI brain,
 * proving the seam the whole architecture rests on.
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
	void HandleTogglePossession(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY()
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY()
	TObjectPtr<UInputAction> ToggleAction;

	/** The agent we handed back to its own brain, so P can reclaim it. */
	TWeakObjectPtr<AAnimalCharacter> ReleasedAnimal;
};
