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

	GetCapsuleComponent()->InitCapsuleSize(40.f, 50.f);

	// The body faces where it runs; the camera is free to look around it.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));

	// Grey-box wolf: an elongated body box and a head box, from engine content only.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	BodyMesh->SetRelativeScale3D(FVector(1.1f, 0.45f, 0.45f));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetCapsuleComponent());
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(65.f, 0.f, 5.f));
	HeadMesh->SetRelativeScale3D(FVector(0.35f, 0.3f, 0.3f));

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
		FSoftObjectPath(TEXT("/Game/Agents/DA_Wolf.DA_Wolf")));
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
	GetCharacterMovement()->MaxWalkSpeed = ResolvedConfig->WalkSpeed;
	Stamina->Configure(*ResolvedConfig);
}

void AAnimalCharacter::SetWantsToSprint(bool bInWantsToSprint)
{
	bWantsToSprint = bInWantsToSprint;
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
		Activity = bSprintActive ? EStaminaActivity::Sprinting : EStaminaActivity::Moving;
	}
	Stamina->Update(Activity, DeltaTime);

	// ...then re-gate: sprint is a continuous negotiation with stamina, not a latch.
	// The moment the body is exhausted the sprint dies mid-stride, whoever drives (H2).
	const bool bShouldSprint = bWantsToSprint && Stamina->CanSprint();
	if (bShouldSprint != bSprintActive)
	{
		bSprintActive = bShouldSprint;
		GetCharacterMovement()->MaxWalkSpeed =
			bSprintActive ? ResolvedConfig->SprintSpeed : ResolvedConfig->WalkSpeed;
	}
}
