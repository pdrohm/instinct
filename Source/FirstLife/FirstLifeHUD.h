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

	/**
	 * Toggle the playtest telemetry overlay (H14 feel-pass aid, key F1): floating per-animal
	 * readouts of condition / stamina / state so the owner can SEE the simulation while tuning
	 * — is the straggler mechanic producing a laggard? is the herd flushing? A dev tool, not
	 * shipping UI. Off by default.
	 */
	void ToggleDebugOverlay() { bShowDebugOverlay = !bShowDebugOverlay; }

private:
	bool bShowDebugOverlay = false;
};
