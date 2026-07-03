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
	const FVector ReindeerSpawns[] = {
		FVector(600.f, 300.f, 120.f),
		FVector(1450.f, 520.f, 120.f),
		FVector(950.f, 1400.f, 120.f),
		FVector(1900.f, 1150.f, 120.f),
		FVector(300.f, 1250.f, 120.f),
		FVector(1600.f, -150.f, 120.f),
		FVector(2350.f, 600.f, 120.f),
		FVector(1050.f, -550.f, 120.f),
	};

	/**
	 * The saiga herd (second species): a TIGHTER, denser cluster of 12 on the far
	 * (−X) side of the greybox floor, well clear of the reindeer at +X so the two
	 * herds read as separate aggregations on sight. Denser spacing than the reindeer
	 * reinforces the saiga's "one twitchy superorganism" flush read (research §2).
	 */
	const FVector SaigaSpawns[] = {
		FVector(-1500.f, 200.f, 120.f),
		FVector(-1750.f, 520.f, 120.f),
		FVector(-1400.f, 720.f, 120.f),
		FVector(-1900.f, 250.f, 120.f),
		FVector(-2150.f, 560.f, 120.f),
		FVector(-1650.f, -150.f, 120.f),
		FVector(-2000.f, -300.f, 120.f),
		FVector(-1350.f, 100.f, 120.f),
		FVector(-2250.f, 150.f, 120.f),
		FVector(-1550.f, 900.f, 120.f),
		FVector(-1950.f, 850.f, 120.f),
		FVector(-2200.f, -50.f, 120.f),
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

	// One shared definition per species (ADR-E4: data, not a subclass). All tuning
	// lives in the factories, none here. Two configs → two herds that never mix.
	ReindeerConfig = UAnimalConfig::CreateReindeerConfig(this);
	SaigaConfig = UAnimalConfig::CreateSaigaConfig(this);

	SpawnHerd(ReindeerConfig, ReindeerSpawns);
	SpawnHerd(SaigaConfig, SaigaSpawns);
}

void AFirstLifeGameMode::SpawnHerd(const UAnimalConfig* Config, TArrayView<const FVector> Spawns)
{
	if (!Config)
	{
		return;
	}

	for (const FVector& Location : Spawns)
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

		Prey->SetConfigOverride(Config);
		Prey->FinishSpawning(SpawnTransform);

		// AutoPossessAI only covers placed actors; spawned ones need the brain
		// handed to them explicitly.
		Prey->SpawnDefaultController();
	}
}
