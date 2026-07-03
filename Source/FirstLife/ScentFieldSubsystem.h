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

	/**
	 * The spoor query (H14 tracking): the freshest scent within Radius of From, returned
	 * as a follow-this-way 2D direction + freshness [0..1] (1 = just laid, 0 = about to
	 * age out). This is how the persistence hunter continues when the quarry breaks line
	 * of sight — reading the trail, not the animal (Liebenberg, *The Art of Tracking*).
	 * Returns false when the trail has gone cold (no point in range) — losing the spoor is
	 * how a hunt fails. Prey-only: the hunter's own body does not lay trackable scent (see
	 * the deposit step), so the freshest nearby point is always the quarry's.
	 */
	bool GetFreshestTrailNear(const FVector& From, float Radius,
		FVector& OutDirection, float& OutFreshness) const;

private:
	TArray<FScentPoint> ScentPoints;
	float TimeUntilNextSample = 0.f;
};
