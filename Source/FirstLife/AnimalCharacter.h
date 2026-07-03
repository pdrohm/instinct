#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AnimalCharacter.generated.h"

class UAnimalConfig;
class UCameraComponent;
class UHungerComponent;
class ULocomotionComponent;
class USpeciesPerceptionComponent;
class USpringArmComponent;
class UStaminaComponent;
class UStaticMeshComponent;

/**
 * A generic animal agent — the playable early human is just this body with a human
 * config, exactly as the wolf was before it (H7). AI drives it by default; player
 * control is a possession layer on top. Nothing in here knows who the controller is.
 *
 * The character owns no movement logic: gait resolution and the energy economy
 * live in ULocomotionComponent + UStaminaComponent, driven by the species'
 * UAnimalConfig. This class just wires body, brain seam, and camera together.
 */
UCLASS()
class FIRSTLIFE_API AAnimalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAnimalCharacter();

	/**
	 * Down this animal — the terminal result of a successful catch (H14). Only ever
	 * called by UHuntSubsystem on prey, never on the player: downing is not a status
	 * effect the body toggles on itself, it is the outcome the hunt inflicts. A downed
	 * pawn's brain goes silent (see AAnimalAIController::Tick) and its body collapses
	 * via the posture telegraph in Tick().
	 */
	void SetDowned(bool bInDowned);

	bool IsDowned() const { return bDowned; }

	/** Intent, not state: sprint only actually happens while stamina allows (H2). */
	void SetWantsToSprint(bool bInWantsToSprint);

	/** Walk is the slow recover/stalk gait; the default gait is the species' preferred one. */
	void SetWantsToWalk(bool bInWantsToWalk);

	bool IsSprinting() const;

	/** World-space yaw of the fixed isometric camera — the frame of reference for screen-relative movement input (D14). */
	float GetCameraYaw() const;

	UStaminaComponent* GetStamina() const { return Stamina; }

	UHungerComponent* GetHunger() const { return Hunger; }

	ULocomotionComponent* GetLocomotion() const { return Locomotion; }

	USpeciesPerceptionComponent* GetPerception() const { return Perception; }

	USpringArmComponent* GetCameraBoom() const { return SpringArm; }

	UCameraComponent* GetCamera() const { return Camera; }

	const UAnimalConfig* GetConfig() const { return ResolvedConfig; }

	/**
	 * Inject a resolved species config before BeginPlay, overriding ConfigAsset.
	 * This is how one pawn class expresses many species without a subclass (ADR-E4):
	 * the herd spawner hands each prey a runtime-built reindeer UAnimalConfig. Must be
	 * called before BeginPlay resolves the config — use SpawnActorDeferred +
	 * FinishSpawning. Passing nullptr is a no-op (the ConfigAsset path is used).
	 */
	void SetConfigOverride(const UAnimalConfig* InConfig) { ConfigOverride = InConfig; }

	/**
	 * Inject this individual's body condition [~0.78..1.0] before BeginPlay (H14 straggler).
	 * Predators crop the substandard: prime adults escape, the hunt succeeds against the one
	 * already compromised (Mech, Peterson & Page; FitzGibbon; Liebenberg). Condition fully
	 * scales the stamina reserve and lightly trims top speed, so a poor individual keeps up
	 * on the first flush but empties faster and drifts to the rear under pressure — the
	 * straggler self-sorts, no herd-brain change needed. The herd spawner rolls it per prey;
	 * the player body is left at 1.0 (you are not a random weakling). Clamped in ApplyConfig.
	 */
	void SetConditionOverride(float InCondition) { Condition = InCondition; }

	/** This individual's body condition [0.78..1.0] (H14 straggler). Read by the debug overlay. */
	float GetCondition() const { return Condition; }

protected:
	virtual void BeginPlay() override;

	/**
	 * Drives the CONTINUOUS posture telegraph (H14). The head-drop reads fatigue off
	 * stamina every frame (1 − staminaFraction) so a tiring animal visibly sinks its
	 * nose toward the ground — the readable "this one is blowing" tell that precedes the
	 * catch window — and a downed body buckles and rolls to the floor. Everything eases
	 * via FInterpTo so posture flows rather than pops.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Species definition. Defaults to /Game/Agents/DA_Human; when the asset does not
	 * exist yet, falls back to UAnimalConfig class defaults so the slice runs with
	 * zero binary assets.
	 */
	UPROPERTY(EditAnywhere, Category = "Agent")
	TSoftObjectPtr<UAnimalConfig> ConfigAsset;

	UPROPERTY(VisibleAnywhere, Category = "Agent")
	TObjectPtr<UStaminaComponent> Stamina;

	UPROPERTY(VisibleAnywhere, Category = "Agent")
	TObjectPtr<UHungerComponent> Hunger;

	UPROPERTY(VisibleAnywhere, Category = "Agent")
	TObjectPtr<ULocomotionComponent> Locomotion;

	UPROPERTY(VisibleAnywhere, Category = "Agent")
	TObjectPtr<USpeciesPerceptionComponent> Perception;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, Category = "Body")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Body")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

private:
	void ApplyConfig();

	/** Optional pre-BeginPlay species override (herd-spawn path). Wins over ConfigAsset. */
	UPROPERTY(Transient)
	TObjectPtr<const UAnimalConfig> ConfigOverride;

	UPROPERTY(Transient)
	TObjectPtr<const UAnimalConfig> ResolvedConfig;

	/** True once caught. Never set on the player — see SetDowned(). */
	bool bDowned = false;

	/** This individual's body condition (H14 straggler); 1.0 = prime adult. See SetConditionOverride(). */
	float Condition = 1.f;

	// Base posture, captured from the resolved config in ApplyConfig() so the telegraph
	// is relative to whatever species silhouette is active (ADR-E4). Tick() lerps the
	// live mesh offsets away from these and eases back toward them as fatigue clears.
	FVector HeadBaseOffset = FVector::ZeroVector;
	FVector BodyBaseOffset = FVector::ZeroVector;
};
