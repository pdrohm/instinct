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
#include "StaminaComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	/** Below this ground speed (cm/s) the animal counts as resting. */
	constexpr float RestSpeedThreshold = 20.f;
}

AAnimalCharacter::AAnimalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Humanoid proportions: ~180cm standing figure.
	GetCapsuleComponent()->InitCapsuleSize(34.f, 90.f);

	// The body faces where it runs; the camera is free to look around it.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));

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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 450.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 80.f);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

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
	// Default gait is the sustainable run; walk and sprint are deliberate choices.
	GetCharacterMovement()->MaxWalkSpeed = ResolvedConfig->RunSpeed;
	Stamina->Configure(*ResolvedConfig);
}

void AAnimalCharacter::SetWantsToSprint(bool bInWantsToSprint)
{
	bWantsToSprint = bInWantsToSprint;
}

void AAnimalCharacter::SetWantsToWalk(bool bInWantsToWalk)
{
	bWantsToWalk = bInWantsToWalk;
}

void AAnimalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ResolvedConfig)
	{
		return;
	}

	// Charge for the state the body was actually in this frame...
	const float GroundSpeed = GetVelocity().Size2D();
	EStaminaActivity Activity = EStaminaActivity::Resting;
	if (GroundSpeed > RestSpeedThreshold)
	{
		if (bSprintActive)
		{
			Activity = EStaminaActivity::Sprinting;
		}
		else
		{
			Activity = bWantsToWalk ? EStaminaActivity::Moving : EStaminaActivity::Running;
		}
	}
	Stamina->Update(Activity, DeltaTime);

	// ...then re-gate: sprint is a continuous negotiation with stamina, not a latch.
	// The moment the body is exhausted the sprint dies mid-stride, whoever drives (H2).
	bSprintActive = bWantsToSprint && Stamina->CanSprint();

	// Resolve the gait: sprint > walk > run (the human's default, sustainable pace).
	float DesiredSpeed = ResolvedConfig->RunSpeed;
	if (bSprintActive)
	{
		DesiredSpeed = ResolvedConfig->SprintSpeed;
	}
	else if (bWantsToWalk)
	{
		DesiredSpeed = ResolvedConfig->WalkSpeed;
	}
	if (GetCharacterMovement()->MaxWalkSpeed != DesiredSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = DesiredSpeed;
	}
}
