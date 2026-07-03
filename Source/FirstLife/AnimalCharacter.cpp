#include "AnimalCharacter.h"

#include "AnimalAIController.h"
#include "AnimalConfig.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "FirstLife.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LocomotionComponent.h"
#include "SpeciesPerceptionComponent.h"
#include "StaminaComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	// Fixed isometric rig (D14). Angle/distance/FOV are first-feel defaults for the
	// open camera questions (Q15 distance, Q16 rotation) — provisional until the
	// hands-on pass, like the locomotion numbers in D13.
	constexpr float IsoCameraPitch = -52.5f;
	constexpr float IsoCameraYaw = -45.f;
	constexpr float IsoCameraDistance = 2600.f;
	constexpr float IsoCameraFieldOfView = 45.f;
	constexpr float IsoCameraLagSpeed = 8.f;
}

AAnimalCharacter::AAnimalCharacter()
{
	// Movement thinks in ULocomotionComponent's tick; the actor itself has nothing to do.
	PrimaryActorTick.bCanEverTick = false;

	// Humanoid proportions: ~180cm standing figure.
	GetCapsuleComponent()->InitCapsuleSize(34.f, 90.f);

	// The body faces where it runs; the camera is free to look around it.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
	Locomotion = CreateDefaultSubobject<ULocomotionComponent>(TEXT("Locomotion"));
	Perception = CreateDefaultSubobject<USpeciesPerceptionComponent>(TEXT("Perception"));

	// Grey-box early human: an upright torso box and a head box, from engine content
	// only. Placeholder until a marketplace/Fab humanoid is brought in (spec: art is
	// acquired, never modeled).
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -14.f));
	BodyMesh->SetRelativeScale3D(FVector(0.38f, 0.5f, 1.44f));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(4.f, 0.f, 74.f));
	HeadMesh->SetRelativeScale3D(FVector(0.24f, 0.24f, 0.26f));

	if (CubeMesh.Succeeded())
	{
		BodyMesh->SetStaticMesh(CubeMesh.Object);
		HeadMesh->SetStaticMesh(CubeMesh.Object);
	}

	// Fixed isometric eye over the fully 3D scene (D14). Absolute rotation: the body
	// turns to face where it runs, the camera never turns with it — no orbit, no zoom,
	// no tactical controls yet (Q15/Q16). The narrow FOV at long range flattens
	// perspective toward the isometric read; spring-arm lag keeps the follow smooth.
	// Collision test off: terrain between eye and body must never snap-zoom the view —
	// occlusion handling is deferred with Q16.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(IsoCameraPitch, IsoCameraYaw, 0.f));
	SpringArm->TargetArmLength = IsoCameraDistance;
	SpringArm->SocketOffset = FVector::ZeroVector;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = IsoCameraLagSpeed;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetFieldOfView(IsoCameraFieldOfView);

	// The agent's own brain. Only spawned when no player holds the body (H7 seam).
	AIControllerClass = AAnimalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	ConfigAsset = TSoftObjectPtr<UAnimalConfig>(
		FSoftObjectPath(TEXT("/Game/Agents/DA_Human.DA_Human")));
}

void AAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	ResolvedConfig = ConfigAsset.LoadSynchronous();
	if (!ResolvedConfig)
	{
		ResolvedConfig = GetDefault<UAnimalConfig>();
		UE_LOG(LogFirstLife, Log,
			TEXT("%s: no config asset found, using UAnimalConfig class defaults"), *GetName());
	}

	ApplyConfig();
}

void AAnimalCharacter::ApplyConfig()
{
	Stamina->Configure(ResolvedConfig->MaxStamina, ResolvedConfig->ExhaustionRecoveryFraction);
	Locomotion->Configure(*ResolvedConfig);
}

void AAnimalCharacter::SetWantsToSprint(bool bInWantsToSprint)
{
	Locomotion->SetWantsToSprint(bInWantsToSprint);
}

void AAnimalCharacter::SetWantsToWalk(bool bInWantsToWalk)
{
	Locomotion->SetWantsToWalk(bInWantsToWalk);
}

bool AAnimalCharacter::IsSprinting() const
{
	return Locomotion->IsSprinting();
}

float AAnimalCharacter::GetCameraYaw() const
{
	return SpringArm->GetComponentRotation().Yaw;
}
