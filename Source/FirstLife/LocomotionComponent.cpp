#include "LocomotionComponent.h"

#include "AnimalConfig.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StaminaComponent.h"

namespace
{
	/** Below this ground speed (cm/s) the animal counts as resting. */
	constexpr float RestSpeedThreshold = 20.f;
}

ULocomotionComponent::ULocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Before movement consumes this frame's input, so gait changes apply same-frame.
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void ULocomotionComponent::Configure(const UAnimalConfig& InConfig)
{
	Config = &InConfig;

	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	Movement = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
	Stamina = GetOwner()->FindComponentByClass<UStaminaComponent>();

	if (Movement)
	{
		// Body constants: how this species carries its mass.
		Movement->RotationRate = FRotator(0.f, Config->TurnRateDegPerSecond, 0.f);
		Movement->BrakingDecelerationWalking = Config->BrakingDeceleration;

		ActiveGait = Config->PreferredGait;
		ApplyGaitToBody(GetGaitSettings(ActiveGait));
	}
}

const FGaitSettings& ULocomotionComponent::GetGaitSettings(ELocomotionGait Gait) const
{
	switch (Gait)
	{
	case ELocomotionGait::Walk:
		return Config->Walk;
	case ELocomotionGait::Sprint:
		return Config->Sprint;
	case ELocomotionGait::Jog:
	default:
		return Config->Jog;
	}
}

ELocomotionGait ULocomotionComponent::ResolveGait() const
{
	// Sprint is a continuous negotiation with stamina, not a latch: the moment
	// the body is exhausted the sprint dies mid-stride, whoever drives (H2).
	if (bWantsToSprint && Stamina->CanSprint())
	{
		return ELocomotionGait::Sprint;
	}
	if (bWantsToWalk)
	{
		return ELocomotionGait::Walk;
	}
	// No intent: the species settles into its own biomechanics (human = jog).
	return Config->PreferredGait;
}

void ULocomotionComponent::ApplyGaitToBody(const FGaitSettings& Gait)
{
	if (Movement->MaxWalkSpeed != Gait.MaxSpeed)
	{
		Movement->MaxWalkSpeed = Gait.MaxSpeed;
	}
	if (Movement->MaxAcceleration != Gait.Acceleration)
	{
		Movement->MaxAcceleration = Gait.Acceleration;
	}
}

void ULocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Config || !Movement || !Stamina)
	{
		return;
	}

	// Charge for the gait the body was actually in this frame: a stationary
	// "sprinter" rests; a moving jogger earns the jog's slow recovery.
	bResting = GetOwner()->GetVelocity().Size2D() <= RestSpeedThreshold;
	const float StaminaRate = bResting
		? Config->RestRegenPerSecond
		: GetGaitSettings(ActiveGait).StaminaDeltaPerSecond;
	Stamina->Update(StaminaRate, DeltaTime);

	// Then re-resolve intent against the body's new energy state.
	ActiveGait = ResolveGait();
	ApplyGaitToBody(GetGaitSettings(ActiveGait));
}
