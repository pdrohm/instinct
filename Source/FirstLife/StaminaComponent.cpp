#include "StaminaComponent.h"

#include "AnimalConfig.h"

void UStaminaComponent::Configure(const UAnimalConfig& Config)
{
	Max = Config.MaxStamina;
	SprintDrainPerSecond = Config.SprintDrainPerSecond;
	RunRegenPerSecond = Config.RunRegenPerSecond;
	WalkRegenPerSecond = Config.WalkRegenPerSecond;
	RestRegenPerSecond = Config.RestRegenPerSecond;
	ExhaustionRecoveryFraction = Config.ExhaustionRecoveryFraction;

	Current = Max;
	bExhausted = false;
}

void UStaminaComponent::Update(EStaminaActivity Activity, float DeltaSeconds)
{
	switch (Activity)
	{
	case EStaminaActivity::Sprinting:
		Current -= SprintDrainPerSecond * DeltaSeconds;
		break;
	case EStaminaActivity::Moving:
		Current += WalkRegenPerSecond * DeltaSeconds;
		break;
	case EStaminaActivity::Running:
		Current += RunRegenPerSecond * DeltaSeconds;
		break;
	case EStaminaActivity::Resting:
		Current += RestRegenPerSecond * DeltaSeconds;
		break;
	}

	Current = FMath::Clamp(Current, 0.f, Max);

	if (!bExhausted && Current <= 0.f)
	{
		bExhausted = true;
	}
	else if (bExhausted && Current >= Max * ExhaustionRecoveryFraction)
	{
		bExhausted = false;
	}
}
