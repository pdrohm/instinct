#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AnimalAIController.generated.h"

/**
 * The animal's own (deliberately tiny) brain. Exists so possession is a real seam
 * (H7): releasing the player hands the same body back to an AI. A gentle random
 * wander is the minimum behavior that makes the handoff visible — nothing more
 * belongs here in Slice 1.
 */
UCLASS()
class FIRSTLIFE_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAnimalAIController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	FVector WanderDirection = FVector::ForwardVector;
	float TimeUntilNewDirection = 0.f;
};
