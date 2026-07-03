#include "SpeciesPerceptionProfile.h"

namespace
{
	USpeciesPerceptionProfile* NewProfile(UObject& Outer, const TCHAR* Name)
	{
		USpeciesPerceptionProfile* Profile =
			NewObject<USpeciesPerceptionProfile>(&Outer, NAME_None, RF_Transient);
		Profile->SpeciesName = Name;
		return Profile;
	}
}

TArray<USpeciesPerceptionProfile*> USpeciesPerceptionProfile::BuildDebugProfiles(UObject& Outer)
{
	TArray<USpeciesPerceptionProfile*> Profiles;

	// 1 — Homo sapiens: planner and persistence hunter. Medium field, strong forward
	// clarity, long pattern-recognition reach, nearly nose-blind. The baseline eye.
	{
		USpeciesPerceptionProfile* P = NewProfile(Outer, TEXT("Homo sapiens"));
		P->VisualFieldOfViewDegrees = 170.f;
		P->ForwardClarityDistance = 3200.f;
		P->PeripheralAwarenessDistance = 1500.f;
		P->MotionDetectionStrength = 0.35f;
		P->SmellStrength = 0.05f;
		P->HearingStrength = 1300.f;
		P->ScentVisibility = 0.f;
		P->NightVisionStrength = 0.f;
		P->CameraDistanceModifier = 1.f;
		P->CameraHeightModifier = 1.f;
		P->OverlayStyle = EPerceptionOverlayStyle::Tactical;
		Profiles.Add(P);
	}

	// 2 — Deer: constantly alert prey. Near-wraparound awareness, weak forward
	// detail, everything that moves screams. Should feel exposed and reactive.
	{
		USpeciesPerceptionProfile* P = NewProfile(Outer, TEXT("Deer"));
		P->VisualFieldOfViewDegrees = 310.f;
		P->ForwardClarityDistance = 1400.f;
		P->PeripheralAwarenessDistance = 3400.f;
		P->MotionDetectionStrength = 1.f;
		P->SmellStrength = 0.35f;
		P->HearingStrength = 2600.f;
		P->ScentVisibility = 0.f;
		P->NightVisionStrength = 0.2f;
		P->CameraDistanceModifier = 1.15f;
		P->CameraHeightModifier = 1.12f;
		P->OverlayStyle = EPerceptionOverlayStyle::Prey;
		Profiles.Add(P);
	}

	// 3 — Wolf: tracker. Medium vision, low-light edge, and the nose — scent trails
	// and wind become visible. Reading invisible information is the fantasy.
	{
		USpeciesPerceptionProfile* P = NewProfile(Outer, TEXT("Wolf"));
		P->VisualFieldOfViewDegrees = 220.f;
		P->ForwardClarityDistance = 2200.f;
		P->PeripheralAwarenessDistance = 2400.f;
		P->MotionDetectionStrength = 0.7f;
		P->SmellStrength = 1.f;
		P->HearingStrength = 2100.f;
		P->ScentVisibility = 1.f;
		P->NightVisionStrength = 0.5f;
		P->CameraDistanceModifier = 1.f;
		P->CameraHeightModifier = 1.f;
		P->OverlayStyle = EPerceptionOverlayStyle::Scent;
		Profiles.Add(P);
	}

	// 4 — Big cat (saber-tooth placeholder): ambush predator. Narrow, deep, forward
	// cone with hard target focus. Powerful — but the information is directional.
	// FOV tightened 115 → 70 for the knowledge-gating slice: with two rungs the cone
	// IS the tunnel, and 115° read too generous to feel flankable in testing terms.
	{
		USpeciesPerceptionProfile* P = NewProfile(Outer, TEXT("Big cat"));
		P->VisualFieldOfViewDegrees = 70.f;
		P->ForwardClarityDistance = 3600.f;
		P->PeripheralAwarenessDistance = 1000.f;
		P->MotionDetectionStrength = 0.9f;
		P->SmellStrength = 0.2f;
		P->HearingStrength = 1600.f;
		P->ScentVisibility = 0.f;
		P->NightVisionStrength = 0.65f;
		P->CameraDistanceModifier = 0.9f;
		P->CameraHeightModifier = 0.85f;
		P->OverlayStyle = EPerceptionOverlayStyle::Ambush;
		Profiles.Add(P);
	}

	return Profiles;
}
