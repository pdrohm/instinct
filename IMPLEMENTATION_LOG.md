# Implementation Log — The First Life

## Session 2026-07-03 (B) — Locomotion becomes a core pillar

> Full architecture: `docs/LOCOMOTION.md`. Decision record: `docs/DESIGN_DECISIONS.md` D13.

- **Refactor:** gait logic extracted from `AAnimalCharacter::Tick` into a new
  `ULocomotionComponent` (ticks TG_PrePhysics; character no longer ticks). Controllers
  still only express intent via the unchanged `SetWantsToSprint/SetWantsToWalk` seam (H7).
- **Data model:** `UAnimalConfig` reshaped into a species locomotion profile —
  per-gait `FGaitSettings { MaxSpeed, Acceleration, StaminaDeltaPerSecond }` for
  Walk/Jog/Sprint (new `LocomotionTypes.h`), plus `PreferredGait`, `TurnRateDegPerSecond`,
  `BrakingDeceleration`, `MaxStamina`, `RestRegenPerSecond`, `ExhaustionRecoveryFraction`.
- **Gait rename:** the default gait is now *Jog* (was "Run") to match the design language.
- **Jog regenerates (+4/s)** — supersedes the previous session's stamina-neutral run, per
  design direction: recovering while moving is the Homo sapiens identity. Order: rest 22 >
  walk 10 > jog 4 > sprint −12 (~8 s burst; exhaustion locks sprint until 30%).
- **Sprint acceleration** now data-driven and high (2400 vs jog 1200 / walk 768).
- **`UStaminaComponent` simplified** to a pure signed-rate reservoir + exhaustion latch
  (`EStaminaActivity` retired); locomotion — and later heat/hunger — decide the rates.
- **HUD:** gait label (RESTING/WALK/JOG/SPRINT) under the stamina bar.
- **Inputs unchanged:** WASD jog, Shift sprint, Ctrl walk, P possession toggle.
- **`DA_Human.uasset`** predates the schema change; orphaned old floats are ignored and new
  struct defaults are the human profile — behavior identical. Re-save to clean (see doc).
- **Compile status:** `Result: Succeeded` (FirstLifeEditor, Mac, Development).

---

> Session 2026-07-03 — Homo sapiens pivot, build unblock, and 3-gait locomotion.
> Companion to `docs/BUILD_LOG.md` (evidence log) and `docs/PROTOTYPE_SPEC.md` (scope contract).

## What was implemented

### 1. Toolchain unblock (UE 5.6 now compiles on this Mac)
- **Metal Toolchain** installed (`xcodebuild -downloadComponent MetalToolchain`) — Xcode 26 unbundles it.
- **UBT SDK gate override**: `/Users/Shared/Epic Games/UE_5.6/Engine/Config/Apple/Apple_SDK.json`
  `MaxVersion` `16.9.0` → `26.2.0`. **Re-apply after any engine reinstall/verify** (engine-side file,
  reset by the installer). Community-proven for Xcode 26.2.
- **Lumen disabled** in `Config/DefaultEngine.ini` (`r.DynamicGlobalIlluminationMethod=0`,
  `r.ReflectionMethod=2`): Lumen's Lit pass rendered black on this UE 5.6 + macOS 26 combo.
  Classic deferred + SSR works with the map's movable sun/skylight.
- Fixed a `-Wshadow` error (`WolfPlayerController.cpp` — Xcode 26 clang is stricter).

### 2. Species pivot: wolf → early Homo sapiens (docs + code)
- All design docs updated (see `docs/BUILD_LOG.md` 2026-07-03 entry for the decision record).
- `UAnimalConfig` class defaults are now the **human endurance profile**:
  Walk 200 / Run 400 / Sprint 620 cm/s; sprint drain 12/s (~8 s burst), run neutral (0 regen),
  walk regen 10/s, rest regen 22/s, exhaustion lock until 30%.
- Placeholder body is an upright grey-box humanoid (~180 cm; capsule 34×90) — engine cubes only.
- Character soft-loads `/Game/Agents/DA_Human` (falls back to class defaults if missing).

### 3. Three gaits: Walk / Run / Sprint (priority items 4–6)
- **Run** = default gait, stamina-neutral. This *is* the persistence-hunting identity (H2/H14):
  sustainable forever, faster than prey recovery, slower than prey sprint (prey come in slice 2+).
- **Walk** (hold **Left Ctrl**) = slow stalk/recover gait — regenerates stamina while moving.
- **Sprint** (hold **Left Shift**) = burst; drains; dies mid-stride at exhaustion; locked until 30% recovery.
- New `EStaminaActivity::Running`; gait resolution lives in `AAnimalCharacter::Tick`.
- Stamina UI (canvas HUD bar: white → amber → red) carried over unchanged (priority 7 already existed).

### 4. Content
- `Content/Agents/DA_Human.uasset` created (headless script run, verified on disk).
- `Content/Maps/GreyBox.umap` verified loading headlessly. Map satisfies priority 8:
  100×100 m floor, boundary walls, 3 rocks + a rise (LOS breakers/elevation), PlayerStart,
  movable sun + real-time skylight + atmosphere (no lighting build ever needed).

## Changed files

| File | Change |
|---|---|
| `Source/FirstLife/AnimalConfig.h` | +RunSpeed, +RunRegenPerSecond; human default tuning |
| `Source/FirstLife/StaminaComponent.h/.cpp` | +`Running` activity (neutral); reads new config field |
| `Source/FirstLife/AnimalCharacter.h/.cpp` | +SetWantsToWalk; 3-gait resolution; upright humanoid placeholder; `DA_Human` path |
| `Source/FirstLife/WolfPlayerController.h/.cpp` | +Walk input (Left Ctrl); doc comments; shadow fix |
| `Scripts/build_greybox_map.py` | Creates `DA_Human` instead of `DA_Wolf` |
| `Config/DefaultEngine.ini` | Renderer overrides (Lumen off → GI None + SSR) |
| `docs/VISION.md`, `docs/PROTOTYPE_SPEC.md`, `docs/HYPOTHESES.md`, `docs/DESIGN_DECISIONS.md`, `docs/BUILD_LOG.md` | Species-pivot decision record (incl. new H14) |
| `Content/Agents/DA_Human.uasset` | New (script-generated, class-default tuning) |

**Compile status:** `Result: Succeeded` (FirstLifeEditor, Mac, Development — Mac SDK 26.2).

## How to open and test

1. Double-click `FirstLife.uproject` (or `"/Users/Shared/Epic Games/UE_5.6/Engine/Binaries/Mac/UnrealEditor" FirstLife.uproject`).
2. GreyBox is the startup map. Press **Play**.
3. Controls: **WASD** move (default = endurance run) · **Mouse** look · **Shift** sprint ·
   **Ctrl** walk (stamina recovers while moving) · **P** release/reclaim the body to/from its AI.
4. What to feel for (H2): sprint ~8 s to burn out; exhaustion locks sprint until the bar recovers
   to 30% (amber); walking recovers on the move, resting recovers fastest.
5. Tune without recompiling: edit `Content/Agents/DA_Human` in the editor.

## Known issues

- **Editor was launched before this build** — if an editor is still open, restart it to load the new dylib.
- clangd/IDE shows false errors (`CoreMinimal.h not found`) — no `compile_commands.json`; the real build is green. Generate an Xcode workspace via `GenerateProjectFiles.sh` if IDE IntelliSense is wanted.
- `DA_Wolf.uasset` still exists (stale but harmless — kept for tuning comparison; delete freely).
- `AWolfPlayerController` class name is stale post-pivot — intentionally NOT renamed ("do not restructure"); queued for a natural refactor window.
- Untested by a human hand this session: gait feel/tuning values are first-pass guesses.

## Intentionally NOT implemented (scope wall held)

Prey, herd AI, hunger, crafting, combat, inventory, fire, shelter, multiplayer, save system, menus,
polished art — all explicitly excluded per directive and `PROTOTYPE_SPEC.md`.

**Marketplace humanoid character (priority 9): stopped at the stop-condition.** A believable human
needs a SkeletalMesh + AnimBP — binary assets that cannot be authored as text and are not present in
the project. Recommended acquisition path (≈15 min of GUI work, zero custom art):
1. Fab → add a free "prehistoric/primitive human" or use Epic's **Third Person template mannequin**
   (Create project → migrate `Characters/Mannequins` folder into this project).
2. Point a SkeletalMeshComponent at it on `AAnimalCharacter` (replace the two cube components), retarget
   the template's ABP. Config/tuning needs no change — this is exactly the H7 claim being cheap.

## Assumptions made (safest-reasonable-decision log)

- Walk on **Left Ctrl** (hold), matching Shift-hold sprint semantics — one-key discoverable, no menus.
- Run is stamina-*neutral* (not regenerating): sustainable-but-not-free is the human's biological read and keeps walking meaningful.
- Kept class names (`AWolfPlayerController`) to honor "do not restructure the whole project."
- Kept `DA_Wolf` on disk; the character no longer references it.
- Did not commit — awaiting your review of the pivot.

## Recommended next step

**Hands-on feel pass (H2's actual test):** Play 10 minutes. Does run-by-default + scarce sprint create
a rhythm? Tune `DA_Human` live. Then: acquire the mannequin/marketplace human (above), then slice 2
(the living herd) — where the endurance economy finally gets something to be spent *on* (H14).
