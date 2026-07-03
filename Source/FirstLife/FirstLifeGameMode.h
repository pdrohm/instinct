#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "FirstLifeGameMode.generated.h"

class UAnimalConfig;

/**
 * Wires the slice together: animal agent as the possessed player pawn,
 * possession-layer controller, canvas HUD — and spawns the living reindeer
 * herd (H5): 8 pure AAnimalCharacters sharing one runtime reindeer config.
 */
UCLASS()
class FIRSTLIFE_API AFirstLifeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirstLifeGameMode();

protected:
	/**
	 * Spawns the grazing herd (H5): a loose cluster of AAnimalCharacters on the
	 * greybox floor, each injected with the shared reindeer config before
	 * BeginPlay (SpawnActorDeferred → SetConfigOverride → FinishSpawning) and
	 * handed the default AI brain. No subclass, no species branching (ADR-E4) —
	 * the herd behavior itself lives entirely in AnimalAIController.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * Spawns one grazing herd: deferred-spawns an AAnimalCharacter at each location,
	 * injects the shared species config before BeginPlay, and hands over the AI brain.
	 * Called once per species — the shared config pointer is also the herd-mate identity
	 * the brain discovers by (GetConfig() equality — SLICE2_CONTRACT.md), so two configs
	 * become two herds that never flock together, with no species branching (ADR-E4).
	 */
	void SpawnHerd(const UAnimalConfig* Config, TArrayView<const FVector> Spawns);

	/**
	 * The shared species definitions, built once at BeginPlay (data-as-code via the
	 * UAnimalConfig factories, outer = this) and injected into every prey. UPROPERTY
	 * keeps them alive for the level; pointer identity is the herd-mate key.
	 */
	UPROPERTY(Transient)
	TObjectPtr<UAnimalConfig> ReindeerConfig;

	UPROPERTY(Transient)
	TObjectPtr<UAnimalConfig> SaigaConfig;
};
