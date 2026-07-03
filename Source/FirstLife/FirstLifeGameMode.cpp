#include "FirstLifeGameMode.h"

#include "AnimalCharacter.h"
#include "FirstLifeHUD.h"
#include "WolfPlayerController.h"

namespace
{
	/** Debug perception targets, spread so every range band gets exercised. */
	const FVector PerceptionTargetSpawns[] = {
		FVector(2000.f, 2000.f, 120.f),
		FVector(-2600.f, -1200.f, 120.f),
		FVector(600.f, -2700.f, 120.f),
	};
}

AFirstLifeGameMode::AFirstLifeGameMode()
{
	DefaultPawnClass = AAnimalCharacter::StaticClass();
	PlayerControllerClass = AWolfPlayerController::StaticClass();
	HUDClass = AFirstLifeHUD::StaticClass();
}

void AFirstLifeGameMode::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FVector& Location : PerceptionTargetSpawns)
	{
		AAnimalCharacter* Target = GetWorld()->SpawnActor<AAnimalCharacter>(
			AAnimalCharacter::StaticClass(), Location, FRotator::ZeroRotator, Params);
		if (Target)
		{
			// AutoPossessAI only covers placed actors; spawned ones need the brain
			// handed to them explicitly.
			Target->SpawnDefaultController();
		}
	}
}
