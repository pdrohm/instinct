# Implementation Log — The First Life

## Session 2026-07-03 (E) — Autonomous dev loop, iter 5: lose-and-track spoor (commit `d84390d`)

> The tracking half of the persistence hunt (Liebenberg, *The Art of Tracking*): when the quarry breaks line
> of sight you follow the trail, and a cold trail loses the hunt. Built on the existing `UScentFieldSubsystem`
> breadcrumb field, no new system, no research subagent (grounded inline; cost-conscious — session was already
> high). The player body no longer lays trackable scent, so every fresh point is prey spoor;
> `GetFreshestTrailNear(from, radius)` returns the freshest nearby scent as a follow-this direction + freshness
> `[0..1]` or false when cold. HUD draws a HOT→COLD spoor arrow (projected onto the fixed iso screen via the
> camera-yaw basis) only when the nearest live prey is beyond reveal distance — lose close contact and you
> track; regain it and the cue hides. All first-pass numbers.
>
> **Loop checkpoint (5 iterations, all compile-green, NONE playtested):** core hunt loop → hunger → saiga
> (2nd species) → catchable straggler → lose-and-track. This is a complete, winnable, tracked persistence-hunt
> vertical slice. **The bottleneck is now unambiguously GATE-A (a human feel-pass), not more code** — every
> committed number is a guess, and further breadth on an unvalidated foundation runs against the project's own
> "validate before build" law. Recommend the owner play before the loop spends more.

## Session 2026-07-03 (E) — Autonomous dev loop, iter 4: the catchable straggler (commit `a56b361`)

> The single most important addition to the loop: it makes H14 actually *winnable*. An identical herd
> tires in lockstep, so no individual falls behind. Predation-ecology subagent (Mech, Peterson & Page;
> FitzGibbon; Liebenberg) confirmed the load-bearing fact — predators crop the substandard, prime adults
> escape. Now a per-agent body condition `[0.78..1.0]` (injected before BeginPlay, player stays 1.0) FULLY
> scales the innate stamina reserve and LIGHTLY trims top speed (`0.5 + 0.5·condition`), so a poor animal
> runs with the herd on the first flush but empties sooner and **self-sorts to the rear under pressure —
> no herd-brain change needed**. The spawner rolls a right-shouldered distribution
> (`1 − Square(FRand)·(1−floor)`) so a dozen animals yield only one or two genuine stragglers. New seams:
> `AAnimalCharacter::SetConditionOverride`, `ULocomotionComponent::SetSpeedScale`. Deferred: age/sex classes,
> disease/injury sim, condition-over-time, maternal defense (one static scalar stands in). Still unplayed by
> a human hand — GATE-A remains the gate.

## Session 2026-07-03 (E) — Autonomous dev loop, iters 2–3: hunger + second species

> Same loop (research subagent → implementation → authoritative build → commit), run back-to-back.

- **Iter 2 — Hunger need (commit `08130e7`), the reason to hunt.** Physiologist subagent (Pontzer forager
  energetics, Cahill starvation cascade) grounded the numbers. New `UHungerComponent` (tickless reservoir):
  drains 0.083/s (~20 min empty — honest glycogen-buffer→decline ordering compressed ~144× from ~48 h).
  Metabolism is data (ADR-E4): `UAnimalConfig` gained `MaxHunger`, `HungerDrainPerSecond`, `NutritionValue`
  — nutrition lives on the EATEN animal, so a kill delivers the prey's `NutritionValue` (reindeer 85) with no
  hardcoded feed constant. Starvation→stamina coupling (the death spiral): below well-fed thresholds hunger
  scales stamina **regen** (floor 0.3, degrades first/hardest) and **capacity** (floor 0.5); `UHungerComponent`
  owns the curves, `UStaminaComponent` consumes two opaque scalars via `SetHungerModifiers` ("one battery,
  many consumers" preserved). Hunger is player-only pressure (drains/couples only while `IsPlayerControlled()`),
  so the herd is never penalized. HUD gained an amber→red hunger bar.
- **Iter 3 — Saiga antelope (commit `59d7d8c`), the anti-reindeer.** Wildlife subagent (Bekenov/Milner-Gulland)
  grounded a contrasting prey. `CreateSaigaConfig` (pure data): dashes faster (900) and out-cruises the caribou
  (580) but a shallow tank (80) that refills fast when still (24/s) — you win by tempo, not distance; asymmetry
  held. Smaller/paler/twitchier silhouette. Nutrition 48 (~half a caribou = smaller meal). **Two configs became
  two herds for free** (herd-mate discovery is by `GetConfig()` identity); GameMode spawns a tight 12-strong
  saiga cluster via a new DRY `SpawnHerd(config, spawns)` helper.
- **Next priority (queued):** the herd is still homogeneous, so no catchable straggler emerges — iter 4
  (per-agent condition heterogeneity) is what actually makes H14 winnable, ahead of more breadth.
- **Still unverified by a human hand.** GATE-A (feel-pass) remains the gate on all of it.

## Session 2026-07-03 (E) — Autonomous dev loop, iter 1: persistence-hunt core loop (H14)

> First iteration of the owner-requested self-paced dev loop (research subagent → implementation subagent →
> authoritative build → commit). Built the make-or-break slice: run a reindeer to exhaustion and take it.
> Commit `68304fc`. Companion research: kill-moment grounding brief (wildlife-biology subagent, inline).

- **Research subagent (Wildlife Biologist + Exercise Physiologist)** grounded the *kill moment* so the
  telegraph is honest, not invented. Load-bearing finding: **the exhaustion latch IS the catch window** — a
  blown, hyperthermic, myopathic ungulate that has just stopped is briefly catchable but recovers its escape
  once it cools back to ~30% (capture myopathy; Spraker 1993, Liebenberg 2006). So `Stamina->IsExhausted()`
  already *is* "takeable"; no separate downable state was needed. Also: head-drop should be **continuous**
  with fatigue, and recovery must be tied to standing still only (which it already is) — "deny it rest" is the
  whole mechanism, not "outrun it".
- **Implementation subagent (UE5.6 gameplay)** built, against a pinned contract, four pieces:
  `UHuntSubsystem` (tickable world subsystem, mirrors `UScentFieldSubsystem`) does catch-detection with an
  **interruptible dwell** (~1.5 s within 200 cm of an exhausted prey; stepping out or letting it recover
  resets it — that reset is the win condition) + a time-to-kill clock (H14 metric) surfaced on the HUD;
  `UStaminaComponent::Refill()` is the feed beat; `AAnimalCharacter` now ticks a continuous fatigue head-drop
  (`1 − staminaFraction`, eased) and a downed-collapse pose; `AAnimalAIController` silences a downed pawn's
  brain (early-out + `StopMovement`).
- **Per-species grey-box silhouette** (landed same commit, from the prior turn): a species describes its own
  capsule + torso/head boxes + tint in `UAnimalConfig` (ADR-E4 extended to silhouette). Reindeer = dark
  horizontal quadruped vs the human's upright ochre figure — hunter/herd separate at iso distance before real
  meshes (GATE-C). Posture telegraph is relative to each species' base offsets.
- **Seams held:** no subclasses (ADR-E4), no behavior trees, all cognition still in `FHerdBrain`, the only new
  pawn fields are the downed bool + two posture base offsets. All numbers first-pass guesses, commented.
- **Unverified by a human hand:** the whole loop is compile-green only. GATE-A (feel-pass) still open and now
  has real stakes to test against.

## Session 2026-07-03 (D) — Slice 2: the living herd (P2 + A1), parallel fan-out (Chief Architect)

> First implementation fan-out. Built build-order step 2 (the living reindeer herd, H5) via three parallel
> subagents against a pinned interface contract (`SLICE2_CONTRACT.md`). Companion: `TASK_GRAPH.md` (P2/A1),
> `docs/research/HERD_AND_PREY.md` (tuning), `docs/qa/H5_LIVING_HERD_PROTOCOL.md`.

- **Interface contract pinned first (Chief Architect, inline):** two hazards in the existing code forced
  decisions the parallel agents had to share. (1) Prey config can't be a `.uasset` (no headless binary
  authoring) and `ConfigAsset` is protected → made the ONE serialised seam edit myself (ADR-E7): additive
  `AAnimalCharacter::SetConfigOverride(const UAnimalConfig*)`, resolved before `ConfigAsset` in `BeginPlay`.
  (2) `ResolveGait()` exposes only sprint/walk/preferred — no `SetWantsToJog`; the research table's guess of
  reindeer `PreferredGait = Walk` would make the escape-cruise Jog (520) unreachable, silently breaking the
  persistence-hunt energetics. **Fix with zero API change: reindeer `PreferredGait = Jog`; the brain asserts
  `SetWantsToWalk(true)` to graze.** Both hazards + the gait-mapping table recorded in `SLICE2_CONTRACT.md`.
- **P2 — `gameplay-locomotion-engineer` (owns `AnimalConfig.*`, `FirstLifeGameMode.*`):** `CreateReindeerConfig()`
  factory (data-as-code, ADR-E4 — a `UAnimalConfig` instance, not a subclass) with the research tuning
  values (all flagged first-pass); `AFirstLifeGameMode` now spawns a grazing herd of **8** reindeer via
  `SpawnActorDeferred → SetConfigOverride → FinishSpawning → SpawnDefaultController`, one shared reindeer
  config (UPROPERTY, GC-rooted) whose pointer identity doubles as the herd-mate discovery key. Replaced the
  3 debug perception targets.
- **A1 — `ai-engineer` (owns `AnimalAIController.*` + new `HerdBrain*`):** replaced the wander stub with a
  herd brain — a pure per-agent `FHerdBrain` struct (`HerdBrain.*`, `HerdBrainTypes.h`) fed by one
  world-truth sensing seam (`AnimalAIController::SenseHerd()`, marked for the future U4 Umwelt swap). One
  Reynolds boids core, two weight profiles (graze/flee) blended by a per-agent alarm scalar; contagious
  neighbour-to-neighbour flush (no global flag); predator-directed flight vector; FID as a decision surface
  (approach directness × speed × group size × harassment memory); per-agent nervousness/reaction/weight
  jitter → emergent straggler; breakable cohesion → herd splitting; graceful zero-herd-mate degradation (the
  released player body runs the same brain, H7). Drives the pawn only via the intent API.
- **QA — `qa-engineer` (owns `docs/qa/`):** wrote `H5_LIVING_HERD_PROTOCOL.md` (falsifiable do-nothing /
  approach / pursuit / recovery phases with explicit PASS/FAIL signals) and `SLICE2_SMOKE_CHECK.md`. Source
  untouched. Its independent top-3 defects-to-watch corroborated the contract's risk surface (jog-cruise
  grazing, global-vs-contagious flush, missing straggler).
- **Integration:** `cpp-reviewer` pass → **MERGE-WITH-FIXES**, no CRITICAL/HIGH, no lane violations, no
  ADR-E4/E5 breaks, gait contract verified correct end-to-end. Fixed the one MEDIUM myself (promoted three
  hardcoded brain constants into `FHerdBrainConfig`; derived the alert-band floor from `CalmAlarmThreshold`
  to kill a silent duplication — DoD "all tuning lives in data"). Two LOW nits logged as debt below.
- **Compile status:** `Result: Succeeded` (FirstLifeEditor, Mac Development), both before and after the
  MEDIUM fix. Main is green.
- **Debt logged (LOW, non-reachable defensive nits):** (1) `AnimalAIController::OnPossess` guards `InPawn`
  for the brain init then logs `InPawn->GetName()` unguarded — engine asserts non-null before `OnPossess`,
  so unreachable; drop the redundant ternary or guard the log. (2) `SenseHerd` would misclassify an
  `AAnimalCharacter` whose `BeginPlay` hasn't run (`GetConfig()==nullptr`) as a threat — not reachable with
  synchronous spawn; worth a comment if async/streamed spawning is ever added.
- **Not built (scope wall held):** no hunger, no scent gameplay, no Umwelt/perception code, no prey-vs-prey
  feed (that's P4/H14), no mesh. Reindeer numbers are first-pass guesses staged for the owner's tuning pass.

---

## Session 2026-07-03 (C) — Orchestration bootstrap (Chief Architect)

> No gameplay code written this session — by design. Established the coordination layer and cleared the
> path for parallel delegation. Companion: `PROJECT_STATUS.md`, `TASK_GRAPH.md`, `CURRENT_SPRINT.md`,
> `ARCHITECTURE_DECISIONS.md`, `AGENTS/`.

- **Analysis:** Full read of all design docs + a ground-truth C++ architecture map (delegated). Confirmed:
  species = two data assets on one `AAnimalCharacter` (D13 honored); possession seam clean & demonstrated
  (strong H7 evidence); AI is a wander stub; the belief store exists only in embryo (Absent/Tracked,
  player-body-only) inside `USpeciesPerceptionComponent`; `ScentFieldSubsystem` is a flat FIFO array; zero
  networking.
- **Delegated in parallel (all non-code, safe):** (1) code-architecture map; (2) perception design-consistency
  audit → produced the 4 GATE-B sign-off blockers; (3) herd/prey research → `docs/research/HERD_AND_PREY.md`
  (recommends **reindeer**, with a sourced tuning table).
- **Key finding:** the bottleneck is **validation & sign-off, not engineering capacity.** The next code is
  gated on a hands-on feel-pass (GATE-A), a perception sign-off (GATE-B), and a mesh import (GATE-C) — none
  of which a headless agent can do. Available parallel work today is research + docs, not implementation.
- **Hygiene fix (only code-adjacent change):** removed the `AndroidFileServer` block incl. a checked-in
  `SecurityToken` from `Config/DefaultEngine.ini` (Desktop-only project; template leftover). Config-only.
- **Created:** `PROJECT_STATUS.md`, `TASK_GRAPH.md` (the DAG with gates/tracks/critical path),
  `CURRENT_SPRINT.md`, `ARCHITECTURE_DECISIONS.md` (ADR-E1…E9), and the `AGENTS/` roster (11 specialists,
  disjoint ownership matrix, ready-to-paste prompts).
- **Compile status:** unchanged — no source `.cpp/.h` touched; the config edit does not affect the build.
- **Remaining (owner action):** resolve GATE-A/B/C (see `CURRENT_SPRINT.md`); on each clear, the pre-staged
  agent fan-out fires.

---

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
