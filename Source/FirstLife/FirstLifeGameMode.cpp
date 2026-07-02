#include "FirstLifeGameMode.h"

#include "AnimalCharacter.h"
#include "FirstLifeHUD.h"
#include "WolfPlayerController.h"

AFirstLifeGameMode::AFirstLifeGameMode()
{
	DefaultPawnClass = AAnimalCharacter::StaticClass();
	PlayerControllerClass = AWolfPlayerController::StaticClass();
	HUDClass = AFirstLifeHUD::StaticClass();
}
