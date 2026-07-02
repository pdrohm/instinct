#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "FirstLifeHUD.generated.h"

/**
 * Minimal diegetic-adjacent feedback (H2/H3): one stamina bar drawn straight onto
 * the canvas — no UMG, no assets. The bar is the entire UI of Slice 1 on purpose.
 */
UCLASS()
class FIRSTLIFE_API AFirstLifeHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
