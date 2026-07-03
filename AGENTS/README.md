# AGENTS — Specialist Roster

> The Chief Architect's delegation library. Each file here is a **disposable specialist**: one objective,
> one ownership area, a self-contained ready-to-paste prompt, and a Definition of Done. Spawn one when its
> gate clears (see `TASK_GRAPH.md`), review its output, merge, destroy.
> **Ownership never overlaps.** The matrix below is law — if two agents would touch the same file, the
> Chief Architect serialises them (never runs them concurrently).

---

## Ownership matrix (who may edit what)

| Agent | Owns (may edit) | May NOT touch | Track |
|---|---|---|---|
| `gameplay-locomotion-engineer` | `LocomotionComponent.*`, `StaminaComponent.*`, `AnimalConfig.*`, `LocomotionTypes.h`, `WolfPlayerController.*`, `FirstLifeHUD.*`, `FirstLifeGameMode.*`, `Scripts/build_greybox_map.py` | perception, AI brains, scent field | P |
| `ai-engineer` | `AnimalAIController.*` (+ new brain/state-machine classes) | perception internals, locomotion, config | A |
| `perception-engineer` | `SpeciesPerceptionComponent.*`, `SpeciesPerceptionProfile.*`, new `Umwelt`/belief-store classes | locomotion, AI, HUD, scent field internals | U |
| `rendering-engineer` | fog renderer (post-process material / decal + its driver), perception post-process | belief-store logic, gameplay | U |
| `simulation-engineer` | `ScentFieldSubsystem.*` (→ Stimulus Field) | perception, AI, gameplay | S |
| `environment-engineer` | `Scripts/build_greybox_map.py`, camera-rig constants in `AnimalCharacter.*` (camera only) | components' logic, config values | env |
| `asset-pipeline-engineer` | mesh/anim wiring on `AAnimalCharacter` (mesh components only), `Content/` acquisition notes | gameplay/perception/AI logic | assets 🔒 GUI |
| `research-engineer` | `docs/research/**` only | all code, all other docs | R |
| `documentation-engineer` | `docs/**` (design docs), governance `*.md` on request | all code | docs |
| `qa-engineer` | `docs/qa/**`, playtest protocols; may run build/read code | any source edit | QA |
| `integration-architect` | review + merge only; may edit `AAnimalCharacter.*` to serialise component wiring | net-new features | integ |

**The one shared seam:** `AAnimalCharacter.*` is touched (for wiring) by several tracks. Per ADR-E7, only
the `integration-architect` (i.e. the Chief Architect) edits it, serialising component additions so two
tracks never collide there.

---

## Shared context block (paste into EVERY spawned agent)

> You are a specialist engineer on **"The First Life"**, a UE 5.6 (C++), single-player, isometric-camera
> wildlife-simulation prototype built by a solo dev. Read `CLAUDE.md`, `PROJECT_STATUS.md`,
> `ARCHITECTURE_DECISIONS.md`, and `docs/VISION.md` before doing anything. Non-negotiable laws:
> (1) **Species = data assets on one `AAnimalCharacter` pawn — never a subclass** (ADR-E4/D13).
> (2) **Systems live in components; the character only wires them** (ADR-E5).
> (3) **The player is a possessed AI agent — controllers express intent only** (D7).
> (4) **Gameplay before realism; simulation first; small systems over big features** (`GAME_DNA.md`).
> (5) **Stay in your ownership lane** (see `AGENTS/README.md` matrix). Do not edit files you don't own.
> (6) **Keep main compiling** (`Result: Succeeded`, FirstLifeEditor Mac Development). Build before you finish.
> (7) **Do not expand scope.** If you discover work outside your objective, report it — do not do it.
> Toolchain note: UE 5.6 on this Mac needs the Metal Toolchain + `Apple_SDK.json` MaxVersion=26.2.0 already
> applied; Lumen is disabled (SSR fallback) for a Metal/macOS 26 black-viewport bug.

## Definition-of-Done template (every agent must satisfy)

- [ ] Objective met, and *only* the objective (no scope creep).
- [ ] Only owned files changed (diff proves it).
- [ ] `Result: Succeeded` on a clean build.
- [ ] New tuning lives in data (`UAnimalConfig`/profile), not hardcoded.
- [ ] No `if (species == …)` behaviour branching.
- [ ] A one-paragraph report: what changed, what it validates (H-number), what it deliberately did NOT do,
      and any drift/debt discovered.

## Review gate (Chief Architect applies before merge)

Reject on: architectural drift (violates an ADR), duplicated logic, unnecessary complexity, scope creep,
DNA violation (`DESIGN_PRINCIPLES.md` verdict rule), out-of-lane edits, or a red build. Otherwise merge and
update `IMPLEMENTATION_LOG.md` + `PROJECT_STATUS.md`.

## Roster files

`gameplay-locomotion.md` · `ai.md` · `perception.md` · `rendering.md` · `simulation.md` ·
`environment.md` · `assets.md` · `research.md` · `documentation.md` · `qa.md` · `integration.md`
