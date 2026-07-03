#include "ScentFieldSubsystem.h"

#include "AnimalCharacter.h"
#include "EngineUtils.h"

namespace
{
	/** Seconds between breadcrumb drops per moving animal. */
	constexpr float SampleIntervalSeconds = 0.75f;
	/** A scent point older than this no longer exists for anyone's nose. */
	constexpr float MaxScentAgeSeconds = 75.f;
	/** Bodies slower than this are not laying fresh scent worth tracking. */
	constexpr float MinEmitterSpeed = 60.f;
	/** How fast scent smears downwind as it ages, cm/s. */
	constexpr float WindDriftSpeed = 6.f;
	/** Hard cap so a long session can never grow the field unbounded. */
	constexpr int32 MaxScentPoints = 800;
	/** Fixed placeholder wind: blows toward +X+Y across the steppe. */
	const FVector PlaceholderWind = FVector(0.7071f, 0.7071f, 0.f);
}

void UScentFieldSubsystem::Tick(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float Now = World->GetTimeSeconds();

	// Age out stale points every tick — cheap, and keeps queries trivial.
	ScentPoints.RemoveAll([Now](const FScentPoint& Point)
	{
		return Now - Point.SpawnTimeSeconds > MaxScentAgeSeconds;
	});

	TimeUntilNextSample -= DeltaTime;
	if (TimeUntilNextSample > 0.f)
	{
		return;
	}
	TimeUntilNextSample = SampleIntervalSeconds;

	for (TActorIterator<AAnimalCharacter> It(World); It; ++It)
	{
		const AAnimalCharacter* Animal = *It;
		if (!Animal || Animal->GetVelocity().Size2D() < MinEmitterSpeed)
		{
			continue;
		}
		if (ScentPoints.Num() >= MaxScentPoints)
		{
			ScentPoints.RemoveAt(0);
		}
		FScentPoint Point;
		Point.Location = Animal->GetActorLocation();
		Point.SpawnTimeSeconds = Now;
		ScentPoints.Add(Point);
	}
}

TStatId UScentFieldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UScentFieldSubsystem, STATGROUP_Tickables);
}

bool UScentFieldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

FVector UScentFieldSubsystem::GetWindDirection() const
{
	return PlaceholderWind;
}

float UScentFieldSubsystem::GetMaxAgeSeconds() const
{
	return MaxScentAgeSeconds;
}

float UScentFieldSubsystem::GetAgeSeconds(const FScentPoint& Point) const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() - Point.SpawnTimeSeconds : 0.f;
}

FVector UScentFieldSubsystem::GetDriftedLocation(const FScentPoint& Point) const
{
	return Point.Location + PlaceholderWind * WindDriftSpeed * GetAgeSeconds(Point);
}
