#include "FirstLifeGameMode.h"

#include "AnimalCharacter.h"
#include "AnimalConfig.h"
#include "FirstLifeHUD.h"
#include "WolfPlayerController.h"

namespace
{
	/**
	 * The herd (H5): 8 reindeer (HERD_AND_PREY.md — mid of the 6–12 spec range)
	 * in a loose grazing cluster, spacing roughly inside the ~800 cm boids
	 * neighbor radius so they read as one herd on first sight. Well inside the
	 * 100×100 m greybox floor centered on the origin; Z = 120 (capsule drop-in).
	 */
	const FVector HerdSpawns[] = {
		FVector(600.f, 300.f, 120.f),
		FVector(1450.f, 520.f, 120.f),
		FVector(950.f, 1400.f, 120.f),
		FVector(1900.f, 1150.f, 120.f),
		FVector(300.f, 1250.f, 120.f),
		FVector(1600.f, -150.f, 120.f),
		FVector(2350.f, 600.f, 120.f),
		FVector(1050.f, -550.f, 120.f),
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

	// One shared species definition for the whole herd (ADR-E4: data, not a
	// subclass). All reindeer tuning lives in the factory, none here.
	ReindeerConfig = UAnimalConfig::CreateReindeerConfig(this);

	for (const FVector& Location : HerdSpawns)
	{
		const FTransform SpawnTransform(FRotator::ZeroRotator, Location);

		// Deferred spawn so the config override lands before BeginPlay resolves
		// the species (SetConfigOverride wins over ConfigAsset — ADR-E7 seam).
		AAnimalCharacter* Prey = GetWorld()->SpawnActorDeferred<AAnimalCharacter>(
			AAnimalCharacter::StaticClass(), SpawnTransform, /*Owner=*/nullptr,
			/*Instigator=*/nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (!Prey)
		{
			continue;
		}

		Prey->SetConfigOverride(ReindeerConfig);
		Prey->FinishSpawning(SpawnTransform);

		// AutoPossessAI only covers placed actors; spawned ones need the brain
		// handed to them explicitly.
		Prey->SpawnDefaultController();
	}
}
