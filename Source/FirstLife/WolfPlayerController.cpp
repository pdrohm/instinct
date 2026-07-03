#include "WolfPlayerController.h"

#include "AnimalCharacter.h"
#include "EnhancedInputComponent.h"
#include "FirstLifeHUD.h"
#include "EnhancedInputSubsystems.h"
#include "FirstLife.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "SpeciesPerceptionComponent.h"

void AWolfPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BuildInputObjects();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (!Input)
	{
		UE_LOG(LogFirstLife, Error,
			TEXT("InputComponent is not a UEnhancedInputComponent - check DefaultInput.ini"));
		return;
	}
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWolfPlayerController::HandleMove);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWolfPlayerController::HandleLook);
	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AWolfPlayerController::HandleSprintStarted);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWolfPlayerController::HandleSprintCompleted);
	Input->BindAction(WalkAction, ETriggerEvent::Started, this, &AWolfPlayerController::HandleWalkStarted);
	Input->BindAction(WalkAction, ETriggerEvent::Completed, this, &AWolfPlayerController::HandleWalkCompleted);
	Input->BindAction(ToggleAction, ETriggerEvent::Started, this, &AWolfPlayerController::HandleTogglePossession);
	Input->BindAction(DebugAction, ETriggerEvent::Started, this, &AWolfPlayerController::HandleToggleDebug);
	for (int32 Index = 0; Index < SpeciesActions.Num(); ++Index)
	{
		Input->BindAction(SpeciesActions[Index], ETriggerEvent::Started, this,
			&AWolfPlayerController::HandleSelectSpecies, Index);
	}
}

void AWolfPlayerController::BuildInputObjects()
{
	MappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Wolf"));

	MoveAction = NewObject<UInputAction>(this, TEXT("IA_Move"));
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	LookAction = NewObject<UInputAction>(this, TEXT("IA_Look"));
	LookAction->ValueType = EInputActionValueType::Axis2D;

	SprintAction = NewObject<UInputAction>(this, TEXT("IA_Sprint"));
	WalkAction = NewObject<UInputAction>(this, TEXT("IA_Walk"));
	ToggleAction = NewObject<UInputAction>(this, TEXT("IA_TogglePossession"));
	DebugAction = NewObject<UInputAction>(this, TEXT("IA_ToggleDebug"));

	// Move: X = right, Y = forward. Swizzle lifts a key's value into Y (forward axis).
	const auto MapMoveKey = [this](const FKey& Key, bool bSwizzleToForward, bool bNegate)
	{
		FEnhancedActionKeyMapping& Mapping = MappingContext->MapKey(MoveAction, Key);
		if (bNegate)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(this));
		}
		if (bSwizzleToForward)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(this));
		}
	};
	MapMoveKey(EKeys::W, /*swizzle*/ true, /*negate*/ false);
	MapMoveKey(EKeys::S, /*swizzle*/ true, /*negate*/ true);
	MapMoveKey(EKeys::D, /*swizzle*/ false, /*negate*/ false);
	MapMoveKey(EKeys::A, /*swizzle*/ false, /*negate*/ true);

	// Look: negate mouse Y so pushing forward looks up (matches engine templates).
	FEnhancedActionKeyMapping& LookMapping = MappingContext->MapKey(LookAction, EKeys::Mouse2D);
	UInputModifierNegate* NegateY = NewObject<UInputModifierNegate>(this);
	NegateY->bX = false;
	NegateY->bZ = false;
	LookMapping.Modifiers.Add(NegateY);

	MappingContext->MapKey(SprintAction, EKeys::LeftShift);
	MappingContext->MapKey(WalkAction, EKeys::LeftControl);
	MappingContext->MapKey(ToggleAction, EKeys::P);
	MappingContext->MapKey(DebugAction, EKeys::F1);

	// Species perception debug switcher: 1 = human, 2 = deer, 3 = wolf, 4 = big cat.
	const FKey SpeciesKeys[] = {EKeys::One, EKeys::Two, EKeys::Three, EKeys::Four};
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(SpeciesKeys); ++Index)
	{
		UInputAction* Action = NewObject<UInputAction>(
			this, *FString::Printf(TEXT("IA_Species%d"), Index + 1));
		SpeciesActions.Add(Action);
		MappingContext->MapKey(Action, SpeciesKeys[Index]);
	}
}

AAnimalCharacter* AWolfPlayerController::GetInhabitedAnimal() const
{
	return Cast<AAnimalCharacter>(GetPawn());
}

void AWolfPlayerController::HandleMove(const FInputActionValue& Value)
{
	AAnimalCharacter* Animal = GetInhabitedAnimal();
	if (!Animal)
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();
	// Screen-relative WASD against the fixed isometric camera (D14): W is always "up
	// the screen". Control rotation is no longer the movement basis — with a fixed
	// camera it would let mouse motion invisibly rotate the WASD frame.
	const FRotator YawRotation(0.f, Animal->GetCameraYaw(), 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Animal->AddMovementInput(Forward, Axis.Y);
	Animal->AddMovementInput(Right, Axis.X);
}

void AWolfPlayerController::HandleLook(const FInputActionValue& /*Value*/)
{
	// Intentionally inert: the isometric camera is fixed (D14) — no free orbit yet.
	// Limited rotation is an open question (Q16); the look-input plumbing stays so a
	// future answer can rebind without re-deriving the input path.
}

void AWolfPlayerController::HandleSprintStarted(const FInputActionValue& Value)
{
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		Animal->SetWantsToSprint(true);
	}
}

void AWolfPlayerController::HandleSprintCompleted(const FInputActionValue& Value)
{
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		Animal->SetWantsToSprint(false);
	}
}

void AWolfPlayerController::HandleWalkStarted(const FInputActionValue& Value)
{
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		Animal->SetWantsToWalk(true);
	}
}

void AWolfPlayerController::HandleWalkCompleted(const FInputActionValue& Value)
{
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		Animal->SetWantsToWalk(false);
	}
}

void AWolfPlayerController::HandleToggleDebug(const FInputActionValue& /*Value*/)
{
	// F1 flips the playtest telemetry overlay on the HUD — a feel-pass aid so the owner can
	// read the simulation (straggler condition, stamina, flee state) while tuning.
	if (AFirstLifeHUD* FirstLifeHUD = GetHUD<AFirstLifeHUD>())
	{
		FirstLifeHUD->ToggleDebugOverlay();
	}
}

void AWolfPlayerController::HandleSelectSpecies(const FInputActionValue& /*Value*/, int32 ProfileIndex)
{
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		if (USpeciesPerceptionComponent* Perception = Animal->GetPerception())
		{
			Perception->SetActiveProfileIndex(ProfileIndex);
		}
	}
}

void AWolfPlayerController::HandleTogglePossession(const FInputActionValue& Value)
{
	// NOTE: Possess/UnPossess/Destroy are authority-gated; this toggle needs a
	// server-side path before any networked milestone. Fine standalone (Slice 1).
	if (AAnimalCharacter* Animal = GetInhabitedAnimal())
	{
		// Hand the body back to its own brain and watch it live without us.
		Animal->SetWantsToSprint(false);
		ReleasedAnimal = Animal;

		UnPossess();
		if (!Animal->GetController())
		{
			Animal->SpawnDefaultController();
		}

		ChangeState(NAME_Spectating);
		SetViewTargetWithBlend(Animal, 0.4f);
		UE_LOG(LogFirstLife, Log, TEXT("Possession released: %s is AI-driven"), *Animal->GetName());
	}
	else if (ReleasedAnimal.IsValid())
	{
		AAnimalCharacter* ReclaimedAnimal = ReleasedAnimal.Get();
		AController* PreviousBrain = ReclaimedAnimal->GetController();

		Possess(ReclaimedAnimal);
		ReleasedAnimal = nullptr;

		if (PreviousBrain && PreviousBrain != this)
		{
			PreviousBrain->Destroy();
		}
		UE_LOG(LogFirstLife, Log, TEXT("Possession reclaimed: player inhabits %s"), *ReclaimedAnimal->GetName());
	}
}
