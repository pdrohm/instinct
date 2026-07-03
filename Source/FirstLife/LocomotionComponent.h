#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocomotionTypes.h"

#include "LocomotionComponent.generated.h"

class UAnimalConfig;
class UCharacterMovementComponent;
class UStaminaComponent;

/**
 * The locomotion model of one agent — where a species' biomechanics
 * (UAnimalConfig gait profile) meet the engine body (CharacterMovementComponent)
 * and the energy reservoir (UStaminaComponent). This is a core gameplay system,
 * not a speed switcher: every frame it charges for the gait the body was
 * actually in, re-negotiates sprint against stamina, and drives speed,
 * acceleration, and turn rate from data.
 *
 * Controllers (player or AI — H7) only ever express *intent* via
 * SetWantsToSprint/SetWantsToWalk. With no intent the body settles into the
 * species' preferred gait: for Homo sapiens the jog, the endurance gait that
 * slowly recovers on the move. Sprint is a purchase; jog is how you travel.
 *
 * Species-agnostic on purpose: a wolf, smilodon, or mammoth plugs in as a new
 * UAnimalConfig instance. See docs/LOCOMOTION.md.
 */
UCLASS(ClassGroup = (FirstLife), meta = (BlueprintSpawnableComponent))
class FIRSTLIFE_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULocomotionComponent();

	/** Bind to the species profile and apply its body constants. Call once config is resolved. */
	void Configure(const UAnimalConfig& InConfig);

	/** Intent, not state: sprint only actually happens while stamina allows (H2). */
	void SetWantsToSprint(bool bInWantsToSprint) { bWantsToSprint = bInWantsToSprint; }

	/** Intent: walk is the deliberate stalk/recover gait. */
	void SetWantsToWalk(bool bInWantsToWalk) { bWantsToWalk = bInWantsToWalk; }

	/**
	 * Per-agent top-speed multiplier for individual condition (H14 straggler): a
	 * poor-condition animal runs only slightly slower flat-out but — via its scaled
	 * stamina reserve elsewhere — empties far faster, so it falls behind under
	 * sustained pressure rather than being obviously slow. 1.0 = a prime adult.
	 */
	void SetSpeedScale(float InSpeedScale) { SpeedScale = FMath::Max(0.01f, InSpeedScale); }

	UFUNCTION(BlueprintPure, Category = "Locomotion")
	ELocomotionGait GetActiveGait() const { return ActiveGait; }

	UFUNCTION(BlueprintPure, Category = "Locomotion")
	bool IsSprinting() const { return ActiveGait == ELocomotionGait::Sprint; }

	/** True while (nearly) stationary — the body is recovering at the rest rate. */
	UFUNCTION(BlueprintPure, Category = "Locomotion")
	bool IsResting() const { return bResting; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	const FGaitSettings& GetGaitSettings(ELocomotionGait Gait) const;
	ELocomotionGait ResolveGait() const;
	/** The fastest gait that does not drain stamina (delta >= 0) — a body's sustainable ceiling when blown. */
	ELocomotionGait FastestSustainableGait() const;
	void ApplyGaitToBody(const FGaitSettings& Gait);

	UPROPERTY(Transient)
	TObjectPtr<const UAnimalConfig> Config;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(Transient)
	TObjectPtr<UStaminaComponent> Stamina;

	bool bWantsToSprint = false;
	bool bWantsToWalk = false;
	bool bResting = true;
	ELocomotionGait ActiveGait = ELocomotionGait::Jog;

	/** Per-agent condition top-speed multiplier (H14 straggler); 1.0 = a prime adult. */
	float SpeedScale = 1.f;
};
