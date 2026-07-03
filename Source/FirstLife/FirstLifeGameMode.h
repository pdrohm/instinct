#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "FirstLifeGameMode.generated.h"

/** Wires the slice together: wolf agent as pawn, possession-layer controller, canvas HUD. */
UCLASS()
class FIRSTLIFE_API AFirstLifeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirstLifeGameMode();

protected:
	/**
	 * Spawns a few wander-brain animals as debug perception targets so every species
	 * mode has something to see, hear, and smell (SPECIES_PERCEPTION.md). They reuse
	 * the existing AI wander — this is NOT the herd (H5) and must not grow into it.
	 */
	virtual void BeginPlay() override;
};
