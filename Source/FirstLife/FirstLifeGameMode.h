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
	 * The one shared reindeer species definition, built once at BeginPlay
	 * (data-as-code via UAnimalConfig::CreateReindeerConfig, outer = this) and
	 * injected into every prey. UPROPERTY keeps it alive for the level; shared
	 * pointer identity is also how the herd brain discovers herd-mates
	 * (GetConfig() equality — SLICE2_CONTRACT.md).
	 */
	UPROPERTY(Transient)
	TObjectPtr<UAnimalConfig> ReindeerConfig;
};
