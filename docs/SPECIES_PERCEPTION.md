# Species Perception — Prototype Notes

> **SUPERSEDED IN FRAMING (2026-07-03):** playtest review identified the abstraction here as wrong — v1 changes how information is *presented*, not what information is *available*. The redesign lives in `PERCEPTION_DESIGN.md` (knowledge-gated perception, "render knowledge, not world"). This file remains as the record of the v1 build and its parts that survive as scaffolding.
> Status: **v2 slice 1 implemented** (2026-07-03) on top of the v1 scaffolding below.
> Purpose: test one question — *can different species experience the same 3D isometric world differently?* (H17, Q18/Q19, and the pillar decision that pulled H17 forward — see the note at the bottom.)
> This is deliberately ugly. Numbers are guesses. The differences are the product under test, not the visuals.

## v2 Slice 1 — knowledge gating (CURRENT BEHAVIOR)

Perception now controls **information availability**, per `PERCEPTION_DESIGN.md`: the screen renders what the body knows, not the world.

- **Two rungs, vision only: Absent / Tracked.** Another animal is on screen only while the active species' vision channel honestly delivers it — inside the field of view, inside clarity range, **and** with clear line of sight (occluders hide; showing a target through a rock would be fabrication). Otherwise it is *gone* — not dimmed, absent.
- **Perceptual persistence: 0.4 s.** A target that just left the channel lingers briefly to damp edge flicker. This is deliberately short — it is *not* the Remembered rung (that's slice 2) and must never grow into a silent wallhack. This is the one place the honesty rule bends, and by how much is written down here.
- **Tuned pass: Homo sapiens (key 1) and Big cat (key 4).** Human: 170° / 3200 — a planner with a real 190° blind field; things behind you genuinely do not exist until you turn or they cross your arc. Big cat: FOV tightened to **70°** / 3600 — everything ahead is clear and deep, everything else is not there. Deer (2) and wolf (3) still switch but play as plain vision-gated animals — their signature channels (peripheral motion → Detected rung, scent → Remembered rung) are later slices; scent breadcrumbs still accumulate but are no longer player-visible.
- **Overlays demoted to debug.** All v1 drawing (cones, rings, markers, scent blobs) now lives behind the console variable **`firstlife.PerceptionDebug 1`** (open console with `` ` ``). Player-facing perception is purely which actors exist on screen. Post-process grading and per-species camera framing remain (presentation support, not information).
- **Gating is possession-scoped.** Only the inhabited body hides anything; pressing P to release restores the full world (the spectator is not an animal). Everything unhides on possession change and teardown.

**How to test:** run the greybox map. As human (1), let a wanderer drift behind you — it should vanish; turn around — it reappears. Stand still and watch one walk behind a rock — it vanishes mid-walk. As big cat (4), fix on one target and let another flank — you should genuinely not know where it went. Litmus: does switching 1 ↔ 4 feel like a different creature, not a different view mode? If the hiding feels *unfair* rather than embodied, write down exactly when and why — that observation is the slice's most valuable output (PERCEPTION_DESIGN.md §9).

## What was implemented

- **`USpeciesPerceptionProfile`** (`SpeciesPerceptionProfile.h/.cpp`) — a data asset defining a species' perception identity: `SpeciesName`, `VisualFieldOfViewDegrees`, `ForwardClarityDistance`, `PeripheralAwarenessDistance`, `MotionDetectionStrength`, `SmellStrength`, `HearingStrength`, `ScentVisibility`, `NightVisionStrength`, `CameraDistanceModifier`, `CameraHeightModifier`, `OverlayStyle`. All behavior reads these values; no species logic is hardcoded beyond the overlay style. The four debug species are seeded in code (`BuildDebugProfiles`) so the build needs zero binary assets — real `UDataAsset` instances can replace them later without a recompile, same pattern as `DA_Human`/`UAnimalConfig`.
- **`USpeciesPerceptionComponent`** (`SpeciesPerceptionComponent.h/.cpp`) — lives on every `AAnimalCharacter`. Holds the active profile, interpolates the camera (distance/pitch modifiers over the fixed D14 rig), applies per-species post-process (saturation, vignette, exposure-as-night-vision), and debug-draws the overlays for the player-controlled body only.
- **`UScentFieldSubsystem`** (`ScentFieldSubsystem.h/.cpp`) — world-level placeholder scent memory. Every moving animal drops breadcrumb scent points; points age out over 75 s and drift downwind as they age. A fixed wind vector stands in for weather.
- **Debug perception targets** — `AFirstLifeGameMode::BeginPlay` spawns three wander-brain `AAnimalCharacter`s so every species mode has something to see, hear, and smell. They reuse the existing wander AI untouched. *They are not the herd (H5) and must not grow into it.*
- **Switcher + HUD** — keys **1–4** swap the active profile at runtime (`WolfPlayerController`); the HUD's top-left line shows the active species. Movement, stamina, input, map, and possession (P) are untouched.

## How to switch species

In game: **1** = Homo sapiens, **2** = Deer, **3** = Wolf, **4** = Big cat. The camera eases to the new framing; overlays and color grading swap immediately.

## What each species mode currently changes

| | Homo sapiens | Deer | Wolf | Big cat |
|---|---|---|---|---|
| Fantasy | planner / persistence hunter | exposed, reactive prey | tracker reading invisible information | directional ambush power |
| Vision cone | 170°, long (3200) | 310°, short (1400) | 220°, medium (2200) | 115°, longest (3600) + inner 40° focus cone |
| Peripheral motion ring | small, quiet | huge (3400), every mover pulses red with a threat line | medium | small — blind behind |
| Scent overlay | none | none | breadcrumb blobs, hot-orange fresh → blue-grey stale, + wind arrow | none |
| Hearing (rings on unseen movers) | 1300 | 2600 | 2100 | 1600 |
| Camera | baseline | further + more top-down | baseline | closer + flatter |
| Post-process | neutral | slight desat, open frame | strong desat, brighter (low-light feel) | heavy vignette tunnel |

Seen targets get a box marker; the big cat's in-focus targets get a hard lock (bigger box + sight line).

## What is fake placeholder logic

Everything below is a stand-in whose only job is to make the *difference* between species readable:

- **Scent** — breadcrumbs on a timer, uniform "odor", linear downwind drift, fixed wind. No diffusion, terrain, moisture, or odor identity.
- **Night vision** — a camera exposure bias. There is no night; no lighting changes.
- **Hearing** — a radius check on mover speed. No occlusion, no loudness model, no sound events.
- **Motion detection** — a speed threshold. No contrast/size/background sensitivity.
- **Camera "height"** — mapped to spring-arm pitch scaling, not a researched framing choice (Q15/Q16 own the real answer).
- **Overlays** — `DrawDebug*` primitives redrawn per frame. The "dim/desaturate outside awareness" idea is approximated *globally* per species; a directional version needs a post-process material (a binary asset), deferred on purpose.
- **Perception values** — first-feel guesses, not researched biology. Deer FOV (~310°) and human FOV (~170°) are roughly right; everything else is tuned for contrast, not accuracy.

## What should be researched later

- Real visual fields, acuity ranges, and motion sensitivity per species (peer-reviewed; ungulate vision, canid olfaction ranges, felid low-light vision) — before any value is called "the species".
- Scent as a real model: wind fields, decay by weather/terrain, odor identity (who, how long ago) — the wolf's Stage 2 system.
- Whether senses should *gate information* (fog-of-war-style, Q18) instead of only *adding overlays* — the current build shows the world fully and annotates it, which flatters readability but undersells embodiment.
- A post-process material pass for directional perception (peripheral desaturation, focus sharpening) once binary assets are acceptable.
- Whether overlays should be diegetic (in-world) or stay symbolic — the H17 "overlay soup vs. immersion" risk is live and untested at this fidelity.

## What should NOT be expanded yet

- No species gameplay: no hunting, stalking bonuses, detection consequences, or AI reactions to being seen/smelled. Perception is *display only* right now.
- No real animal AI — the three targets wander; that is all they are allowed to do.
- The debug targets must not become the herd (H5 is its own slice with its own test).
- No combat, inventory, crafting, day/night, weather, multiplayer, polished art, final UI.
- No per-species bodies/locomotion swaps tied to the switcher — keys 1–4 swap *senses only*, deliberately, so the perception variable is isolated. (Full species swaps are the possession architecture's job, H7, later.)

## Hypothesis-log note

`HYPOTHESES.md` H17 previously said perception overlays must not enter this prototype. The 2026-07-03 direction decision made species perception a core pillar and pulled a *display-only* slice of H17 forward; H17's status is updated accordingly. The scope wall that remains: perception may not become gameplay until the display-only version proves the differences read.
