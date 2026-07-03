# Slice 2 Interface Contract — "The Living Herd" (P2 + A1)

> Pinned by the Chief Architect before the parallel fan-out so `gameplay-locomotion-engineer` (P2)
> and `ai-engineer` (A1) own disjoint files and never collide. This is law for this slice.
> Companion: `docs/research/HERD_AND_PREY.md` (tuning), `AGENTS/README.md` (ownership matrix).

## The one shared seam edit (already made by the Chief Architect — do NOT re-touch)

`AAnimalCharacter` gained a pre-BeginPlay config injector (ADR-E7, serialized through Integration):

```cpp
// AnimalCharacter.h — additive, no existing signature changed
void SetConfigOverride(const UAnimalConfig* InConfig);   // wins over ConfigAsset; nullptr = no-op
// BeginPlay now: ResolvedConfig = ConfigOverride ? ConfigOverride.Get() : ConfigAsset.LoadSynchronous();
```

This is how one pawn class expresses many species without a subclass (ADR-E4). The reindeer is a
**runtime-built `UAnimalConfig` instance**, not a `.uasset` (headless agents can't author binary assets —
same reason `DA_Human` falls back to class defaults today).

## Prey are driven ONLY through the existing intent API (no new coupling)

```cpp
pawn->AddMovementInput(WorldDir, Scale);   // steering (boids output)
pawn->SetWantsToSprint(bool);              // dash
pawn->SetWantsToWalk(bool);                // graze/stalk
pawn->GetConfig();                         // read-only species identity (herd-mate discovery)
```

No teleport, no direct velocity, no new component on `AAnimalCharacter`. If A1 or P2 believes it needs a
component/field on `AAnimalCharacter`, **STOP and report to the Chief Architect** — do not edit that file.

## CRITICAL — the gait mapping (both agents MUST agree on this)

`ULocomotionComponent::ResolveGait()` exposes exactly three reachable gaits:
`wants-sprint → Sprint`, else `wants-walk → Walk`, else `→ PreferredGait`. There is **no `SetWantsToJog`**.

The reindeer needs three gaits reachable — graze(Walk), escape-cruise(Jog), dash(Sprint). To reach Jog
through the unchanged API, the reindeer's **`PreferredGait = Jog`** (NOT Walk as the research table guesses).
Consequence — the brain (A1) drives gaits as:

| Herd state | Intent the brain sets | Resulting gait | Speed (first-pass) |
|---|---|---|---|
| Graze / Alert | `SetWantsToWalk(true)`, no sprint | Walk | 180 cm/s |
| Flee cruise (sustainable) | clear walk **and** sprint intent | Jog (preferred) | 520 cm/s |
| Flee dash (FID breach / panic) | `SetWantsToSprint(true)` | Sprint (while stamina) | 780 cm/s |
| Return-to-graze | re-assert `SetWantsToWalk(true)` | Walk | 180 cm/s |

**A1 responsibility:** the brain must assert `SetWantsToWalk(true)` as its default/graze state — if it
forgets, a grazing herd will cruise at Jog (520) and read as perpetually fleeing.

## Ownership lanes (disjoint — enforced at review)

- **P2 · gameplay-locomotion-engineer** owns: `AnimalConfig.*` (add reindeer config factory — data-as-code,
  all tuning centralized here, NOT in the GameMode), `FirstLifeGameMode.*` (spawn 8–12 prey via
  `SpawnActorDeferred` → `SetConfigOverride(reindeer)` → `FinishSpawning` → `SpawnDefaultController`).
  Holds one shared reindeer `UAnimalConfig*` (UPROPERTY, outer=GameMode) injected into every prey.
- **A1 · ai-engineer** owns: `AnimalAIController.*` + new brain/boids/state-machine classes. Discovers
  herd-mates by `GetConfig()` pointer-equality within neighbor radius; threat = nearest non-herd
  `AAnimalCharacter` by proximity (the player-possessed human). Per-agent **nervousness / reaction-latency /
  boids-weight jitter** live in the brain (this is what makes the straggler). The brain must degrade to solo
  graze/wander with **zero herd-mates** — the released player body runs this same brain (H7 seam).

**Heterogeneity boundary:** per-agent *max-speed/stamina* variation is NOT this slice — if A1 finds it
genuinely needs it, STOP and report (that's a config/seam change). Reaction/nervousness jitter is enough for
the emergent straggler at Slice 2.

## Build

`"/Users/Shared/Epic Games/UE_5.6/Engine/Build/BatchFiles/Mac/Build.sh" FirstLifeEditor Mac Development -project="<repo>/FirstLife.uproject"`.
The Chief Architect runs the single authoritative integrated build. Implementation agents return
compile-ready diffs; do NOT run concurrent full builds (they share one UBT/module and will clobber).
