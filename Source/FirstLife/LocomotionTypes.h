#pragma once

#include "CoreMinimal.h"

#include "LocomotionTypes.generated.h"

/**
 * The three intentional gaits every land species expresses (H2). What differs
 * between species is not the state machine but the numbers each gait carries
 * (FGaitSettings) — and which gait the body prefers when nobody asks for more.
 */
UENUM(BlueprintType)
enum class ELocomotionGait : uint8
{
	/** Slow, deliberate pace — the stalk/recover gait. */
	Walk,
	/** The cruising gait. For Homo sapiens this is the endurance jog — the species' identity. */
	Jog,
	/** Maximum effort. A burst bought with stamina, never sustained. */
	Sprint
};

/**
 * One gait of one species, as pure data. The signed stamina delta is the whole
 * energy philosophy in a single number: a human's jog regenerates (+), a wolf's
 * lope holds or bleeds (0/−), a big cat's sprint hemorrhages (−−). Burst duration
 * is deliberately not a field — it emerges as MaxStamina / |sprint delta|.
 */
USTRUCT(BlueprintType)
struct FGaitSettings
{
	GENERATED_BODY()

	FGaitSettings() = default;

	FGaitSettings(float InMaxSpeed, float InAcceleration, float InStaminaDeltaPerSecond)
		: MaxSpeed(InMaxSpeed)
		, Acceleration(InAcceleration)
		, StaminaDeltaPerSecond(InStaminaDeltaPerSecond)
	{
	}

	/** Top ground speed in this gait, cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float MaxSpeed = 400.f;

	/**
	 * How hard the body pushes toward that speed, cm/s². This is where sprint
	 * explosiveness (big cats) and herbivore momentum (slow build-up) live.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Acceleration = 1200.f;

	/**
	 * Signed stamina change per second while moving in this gait.
	 * Positive regenerates, negative drains, zero is exactly sustainable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaminaDeltaPerSecond = 0.f;
};
