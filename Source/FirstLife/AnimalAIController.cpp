#include "AnimalAIController.h"

#include "AnimalCharacter.h"
#include "AnimalConfig.h"
#include "EngineUtils.h"
#include "FirstLife.h"

AAnimalAIController::AAnimalAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAnimalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Seed the per-agent brain from the body's spawn location as its home graze patch.
	Brain.Init(InPawn ? InPawn->GetActorLocation() : FVector::ZeroVector);
	UE_LOG(LogFirstLife, Log, TEXT("Herd brain took over %s"), *InPawn->GetName());
}

FHerdSense AAnimalAIController::SenseHerd(const AAnimalCharacter& Self) const
{
	// === U4 (Umwelt) SEAM ===
	// This is the ONLY place the brain touches world truth. Today: raw proximity over
	// every AAnimalCharacter (O(n^2) across <=12 actors — fine, no spatial hash/Mass
	// per spec). Later, U4 replaces this whole method with a read of a per-agent belief
	// store; FHerdBrain::Step already depends only on the FHerdSense it returns.
	FHerdSense Sense;

	const UWorld* World = GetWorld();
	if (!World)
	{
		return Sense;
	}

	const UAnimalConfig* MyConfig = Self.GetConfig();
	const FVector MyLocation = Self.GetActorLocation();
	const float NeighborRadiusSq = FMath::Square(BrainConfig.NeighborRadius);

	float NearestThreatDistSq = TNumericLimits<float>::Max();
	const AAnimalCharacter* NearestThreat = nullptr;

	for (TActorIterator<AAnimalCharacter> It(World); It; ++It)
	{
		const AAnimalCharacter* Other = *It;
		if (!Other || Other == &Self)
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(MyLocation, Other->GetActorLocation());

		// Herd-mate = same species identity (GetConfig pointer-equality) within radius.
		// A runtime-built reindeer config is shared across the herd (P2 owns that), so
		// pointer-equality cleanly separates herd from the player-possessed human.
		const bool bSameSpecies = (MyConfig != nullptr) && (Other->GetConfig() == MyConfig);
		if (bSameSpecies)
		{
			if (DistSq <= NeighborRadiusSq)
			{
				FBoidNeighbor Neighbor;
				Neighbor.Location = Other->GetActorLocation();
				Neighbor.Velocity = Other->GetVelocity();
				// Read the neighbor's alarm off its own brain — the contagion substrate.
				if (const AAnimalAIController* OtherBrain =
						Cast<AAnimalAIController>(Other->GetController()))
				{
					Neighbor.Alarm = OtherBrain->GetHerdAlarm();
				}
				Sense.Neighbors.Add(Neighbor);
			}
		}
		else
		{
			// Non-herd animal = candidate threat (the human). Nearest one wins.
			if (DistSq < NearestThreatDistSq)
			{
				NearestThreatDistSq = DistSq;
				NearestThreat = Other;
			}
		}
	}

	if (NearestThreat)
	{
		Sense.bHasThreat = true;
		Sense.ThreatLocation = NearestThreat->GetActorLocation();
		Sense.ThreatVelocity = NearestThreat->GetVelocity();
	}

	return Sense;
}

void AAnimalAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAnimalCharacter* Animal = Cast<AAnimalCharacter>(GetPawn());
	if (!Animal)
	{
		return;
	}

	// 1. SENSE (proximity today; belief store at U4).
	const FHerdSense Sense = SenseHerd(*Animal);

	// 2. THINK (all cognition + tuning is in the brain; identical for AI or player body).
	const FHerdDecision Decision = Brain.Step(
		DeltaTime, Sense, Animal->GetActorLocation(), Animal->GetVelocity(), BrainConfig);

	// 3. APPLY — intent only, never teleport/velocity (SLICE2_CONTRACT).
	Animal->SetWantsToWalk(Decision.bWantsToWalk);
	Animal->SetWantsToSprint(Decision.bWantsToSprint);
	if (Decision.MoveScale > 0.f)
	{
		Animal->AddMovementInput(Decision.MoveDirection, Decision.MoveScale);
	}
}
