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
	// First, what the body is being asked for.
	ELocomotionGait Desired;
	// Sprint is a continuous negotiation with stamina, not a latch: the moment
	// the body is exhausted the sprint dies mid-stride, whoever drives (H2).
	if (bWantsToSprint && Stamina->CanSprint())
	{
		Desired = ELocomotionGait::Sprint;
	}
	else if (bWantsToWalk)
	{
		Desired = ELocomotionGait::Walk;
	}
	else
	{
		// No intent: the species settles into its own biomechanics (human = jog).
		Desired = Config->PreferredGait;
	}

	// Body-truth overrides intent: a blown animal cannot sustain a draining gait.
	// This is what makes endurance a *species identity*, not a number — a human's
	// jog regenerates, so exhaustion never locks it out of travelling; a deer's jog
	// drains, so a blown deer is forced down to its only sustainable gait (the walk)
	// and the persistence hunter closes in. The whole persistence hunt lives here (H14).
	if (Stamina->IsExhausted() && GetGaitSettings(Desired).StaminaDeltaPerSecond < 0.f)
	{
		return FastestSustainableGait();
	}
	return Desired;
}

ELocomotionGait ULocomotionComponent::FastestSustainableGait() const
{
	// The fastest gait that does not drain (delta >= 0). Every species has at least
	// one — the recovery walk — so this always resolves; for a human the jog also
	// qualifies (endurance embodied), for a deer only the walk does.
	ELocomotionGait Best = ELocomotionGait::Walk;
	float BestSpeed = -1.f;
	for (const ELocomotionGait Gait :
		{ ELocomotionGait::Walk, ELocomotionGait::Jog, ELocomotionGait::Sprint })
	{
		const FGaitSettings& Settings = GetGaitSettings(Gait);
		if (Settings.StaminaDeltaPerSecond >= 0.f && Settings.MaxSpeed > BestSpeed)
		{
			BestSpeed = Settings.MaxSpeed;
			Best = Gait;
		}
	}
	return Best;
}

void ULocomotionComponent::ApplyGaitToBody(const FGaitSettings& Gait)
{
	// Per-agent condition trims top speed (H14 straggler): a compromised animal is only
	// slightly slower flat-out — the reserve, scaled elsewhere, is what makes it fall behind.
	const float ScaledSpeed = Gait.MaxSpeed * SpeedScale;
	if (Movement->MaxWalkSpeed != ScaledSpeed)
	{
		Movement->MaxWalkSpeed = ScaledSpeed;
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
