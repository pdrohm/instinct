#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SpeciesPerceptionComponent.generated.h"

class AAnimalCharacter;
class USpeciesPerceptionProfile;

/**
 * The perception seam (H17, Q19): what this body notices about the shared world,
 * and how that is shown to the player who inhabits it. Holds the active
 * USpeciesPerceptionProfile, applies its camera modifiers over the fixed isometric
 * rig (D14), and renders the perception overlays.
 *
 * Deliberately debug-grade: overlays are DrawDebug primitives and camera
 * post-process tweaks, not production VFX. The question under test is whether
 * different profiles make the SAME map feel like a different world — readability
 * first, beauty never (SPECIES_PERCEPTION.md).
 *
 * Movement and stamina are untouched by design: switching perception swaps the
 * senses, not the body. Locomotion identity stays in UAnimalConfig (D13).
 */
UCLASS()
class FIRSTLIFE_API USpeciesPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpeciesPerceptionComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Debug switcher entry point (keys 1-4). Out-of-range indices are ignored. */
	void SetActiveProfileIndex(int32 Index);

	int32 GetActiveProfileIndex() const { return ActiveProfileIndex; }

	int32 GetProfileCount() const { return Profiles.Num(); }

	const USpeciesPerceptionProfile* GetActiveProfile() const;

private:
	AAnimalCharacter* GetAnimalOwner() const;

	/**
	 * Knowledge gating, slice 1 (PERCEPTION_DESIGN.md §10.1): vision channel only,
	 * two rungs. A target the vision channel does not currently deliver is Absent —
	 * hidden outright, not decorated. Runs only for the inhabited body; AI bodies
	 * hide nothing, and releasing possession restores everything.
	 */
	void UpdateKnowledge(float DeltaTime);

	/** True if the vision channel honestly delivers this target: cone + range + line of sight. */
	bool VisionChannelSees(const AAnimalCharacter& Animal,
		const USpeciesPerceptionProfile& Profile, const AAnimalCharacter& Target) const;

	void SetTargetAbsent(AAnimalCharacter& Target, bool bAbsent);

	void RestoreAllHidden();

	/** Targets this observer currently holds Absent (hidden). */
	TSet<TWeakObjectPtr<AAnimalCharacter>> HiddenTargets;

	/** Seconds since the vision channel last delivered each target (perceptual persistence). */
	TMap<TWeakObjectPtr<AAnimalCharacter>, float> SecondsSinceSeen;

	/** Post-process is applied once per switch; camera distance/pitch interp in tick. */
	void ApplyPostProcess() const;
	void InterpCamera(float DeltaTime) const;

	void DrawOverlays() const;
	void DrawVisionArcs(const AAnimalCharacter& Animal, const USpeciesPerceptionProfile& Profile) const;
	void DrawTargetMarkers(const AAnimalCharacter& Animal, const USpeciesPerceptionProfile& Profile) const;
	void DrawScentOverlay(const AAnimalCharacter& Animal, const USpeciesPerceptionProfile& Profile) const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USpeciesPerceptionProfile>> Profiles;

	int32 ActiveProfileIndex = 0;

	/** The D14 rig's resting values, captured at BeginPlay; modifiers scale these. */
	float BaseArmLength = 0.f;
	float BaseArmPitch = 0.f;
};
