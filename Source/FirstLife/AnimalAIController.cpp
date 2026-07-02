#include "AnimalAIController.h"

#include "FirstLife.h"
#include "GameFramework/Pawn.h"

namespace
{
	constexpr float MinSecondsPerDirection = 2.f;
	constexpr float MaxSecondsPerDirection = 5.f;
	/** Fractional input strength: the AI ambles, it does not march. */
	constexpr float WanderInputScale = 0.4f;
}

AAnimalAIController::AAnimalAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAnimalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TimeUntilNewDirection = 0.f;
	UE_LOG(LogFirstLife, Log, TEXT("AI brain took over %s"), *InPawn->GetName());
}

void AAnimalAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	TimeUntilNewDirection -= DeltaTime;
	if (TimeUntilNewDirection <= 0.f)
	{
		const float Angle = FMath::FRandRange(0.f, 2.f * PI);
		WanderDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
		TimeUntilNewDirection = FMath::FRandRange(MinSecondsPerDirection, MaxSecondsPerDirection);
	}

	ControlledPawn->AddMovementInput(WanderDirection, WanderInputScale);
}
