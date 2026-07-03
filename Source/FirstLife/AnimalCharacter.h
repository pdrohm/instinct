#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AnimalCharacter.generated.h"

class UAnimalConfig;
class UCameraComponent;
class USpringArmComponent;
class UStaminaComponent;
class UStaticMeshComponent;

/**
 * A generic animal agent — the playable early human is just this body with a human
 * config, exactly as the wolf was before it (H7). AI drives it by default; player
 * control is a possession layer on top. Nothing in here knows who the controller is.
 */
UCLASS()
class FIRSTLIFE_API AAnimalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAnimalCharacter();

	/** Intent, not state: sprint only actually happens while stamina allows (H2). */
	void SetWantsToSprint(bool bInWantsToSprint);

	/** Walk is the slow recover/stalk gait; default gait (neither walk nor sprint) is the run. */
	void SetWantsToWalk(bool bInWantsToWalk);

	bool IsSprinting() const { return bSprintActive; }

	UStaminaComponent* GetStamina() const { return Stamina; }

	const UAnimalConfig* GetConfig() const { return ResolvedConfig; }

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/**
	 * Species definition. Defaults to /Game/Agents/DA_Human; when the asset does not
	 * exist yet, falls back to UAnimalConfig class defaults so the slice runs with
	 * zero binary assets.
	 */
	UPROPERTY(EditAnywhere, Category = "Agent")
	TSoftObjectPtr<UAnimalConfig> ConfigAsset;

	UPROPERTY(VisibleAnywhere, Category = "Agent")
	TObjectPtr<UStaminaComponent> Stamina;

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

	UPROPERTY(Transient)
	TObjectPtr<const UAnimalConfig> ResolvedConfig;

	bool bWantsToSprint = false;
	bool bWantsToWalk = false;
	bool bSprintActive = false;
};
