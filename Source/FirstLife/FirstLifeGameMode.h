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
};
