#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SpeciesPerceptionProfile.generated.h"

/** Which debug overlay language the species speaks (Q19). Placeholder styles, not final UI. */
UENUM(BlueprintType)
enum class EPerceptionOverlayStyle : uint8
{
	/** Homo sapiens: clear forward tactical reading, nothing exotic. */
	Tactical,
	/** Deer: wide peripheral awareness, motion alarms emphasized. */
	Prey,
	/** Wolf: scent trails + wind — reading invisible information. */
	Scent,
	/** Big cat: narrow forward focus with strong target lock. */
	Ambush
};

/**
 * Data-driven perception identity for one species (H17, Q19) — the sensory sibling
 * of UAnimalConfig's locomotion identity (D13). A species is not "a character with
 * different overlays": it is a different answer to "what do I notice about the same
 * world?", and this asset is where that answer lives. All behavior in
 * USpeciesPerceptionComponent reads these values; nothing species-specific is
 * branched in code beyond the overlay style.
 *
 * The four debug species are code-built (BuildDebugProfiles) so the slice needs zero
 * binary assets — instances of this asset can replace them later without a recompile,
 * exactly like DA_Human replaces UAnimalConfig defaults.
 */
UCLASS(BlueprintType)
class FIRSTLIFE_API USpeciesPerceptionProfile : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName SpeciesName = TEXT("Homo sapiens");

	/** Total horizontal field of view, degrees. Nobody gets 360 — deer gets close. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "10.0", ClampMax = "355.0"))
	float VisualFieldOfViewDegrees = 170.f;

	/** Range (cm) at which things inside the visual field read clearly. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0"))
	float ForwardClarityDistance = 3000.f;

	/** Omnidirectional radius (cm) where movement registers even outside clear vision. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0"))
	float PeripheralAwarenessDistance = 1600.f;

	/** 0..1: how loudly movement outside clear vision announces itself. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MotionDetectionStrength = 0.4f;

	/** 0..1: scent read range multiplier (0 = nose-blind). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smell", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SmellStrength = 0.05f;

	/** Radius (cm) at which movement is heard regardless of direction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hearing", meta = (ClampMin = "0.0"))
	float HearingStrength = 1200.f;

	/** 0..1: how visible the scent overlay is (0 = species cannot read scent at all). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smell", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ScentVisibility = 0.f;

	/** 0..1: low-light adaptation. Placeholder-mapped to camera exposure bias, not real lighting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float NightVisionStrength = 0.f;

	/** Multiplies the base isometric camera distance (D14 rig stays fixed otherwise). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.5", ClampMax = "2.0"))
	float CameraDistanceModifier = 1.f;

	/**
	 * Multiplies the base isometric camera pitch — the placeholder mapping for
	 * "height": >1 looks more top-down (omnidirectional awareness), <1 flatter and
	 * more directional. Q15/Q16 own the real answer.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = "0.5", ClampMax = "1.5"))
	float CameraHeightModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EPerceptionOverlayStyle OverlayStyle = EPerceptionOverlayStyle::Tactical;

	/**
	 * The four debug species (human / deer / wolf / big cat), seeded in code so the
	 * prototype runs with zero binary assets. Values are first-feel guesses to test
	 * whether the *differences* read — not researched numbers (SPECIES_PERCEPTION.md).
	 */
	static TArray<USpeciesPerceptionProfile*> BuildDebugProfiles(UObject& Outer);
};
