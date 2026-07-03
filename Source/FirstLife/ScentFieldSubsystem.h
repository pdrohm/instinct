#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ScentFieldSubsystem.generated.h"

/** One breadcrumb of smell: where a body was, and when. */
USTRUCT()
struct FScentPoint
{
	GENERATED_BODY()

	FVector Location = FVector::ZeroVector;
	float SpawnTimeSeconds = 0.f;
};

/**
 * Placeholder scent memory for the whole world (H17). Every moving animal drops
 * breadcrumb scent points; they age out, and they drift downwind as they age.
 * Species that can read scent (ScentVisibility > 0) render these as debug blobs.
 *
 * FAKE, on purpose: no diffusion, no terrain interaction, no odor types, and wind
 * is a fixed world constant. This exists only to test whether "seeing smell" reads
 * as a different way of knowing the world — the real system is a Stage 2 question.
 */
UCLASS()
class FIRSTLIFE_API UScentFieldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	const TArray<FScentPoint>& GetScentPoints() const { return ScentPoints; }

	/** Unit vector, fixed placeholder wind. The wolf overlay draws it as an arrow. */
	FVector GetWindDirection() const;

	float GetMaxAgeSeconds() const;

	float GetAgeSeconds(const FScentPoint& Point) const;

	/** Where the point currently reads: laid position, drifted downwind by age. */
	FVector GetDriftedLocation(const FScentPoint& Point) const;

private:
	TArray<FScentPoint> ScentPoints;
	float TimeUntilNextSample = 0.f;
};
