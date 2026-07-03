# Slice 2 Smoke Check — build + headless load procedure

> **Owner:** QA authors this checklist. **The Chief Architect owns *running* the authoritative build** —
> the two implementation agents (P2 gameplay-locomotion, A1 ai-engineer) share one UBT/module and a
> concurrent full build will clobber it (`SLICE2_CONTRACT.md`, Build section). **Do NOT run this while
> implementation agents are working.** This document is the pre-flight checklist the Architect executes when
> integrating; QA runs it only on explicit go-ahead, after the fan-out has landed.
>
> **Status:** documented, not yet run. Hold until the Chief Architect confirms the integrated Slice 2 diffs
> are merged and the build lane is clear.

---

## 1. Prerequisites (environment)

Per `PROJECT_STATUS.md` §Toolchain and the project memory, UE 5.6 compiles on this Mac **only after**:

- Metal Toolchain installed.
- `Apple_SDK.json` `MaxVersion` bumped to `26.2.0` (re-apply after every 5.6 reinstall).
- Lumen disabled / SSR fallback in `DefaultEngine.ini` (Metal/macOS 26 black-Lit-viewport bug).

If a fresh engine reinstall happened, confirm these are re-applied *before* building, or the build fails for
environmental reasons unrelated to the Slice 2 code.

---

## 2. Step 1 — Compile the editor target (the authoritative build)

**Command (exact, from `SLICE2_CONTRACT.md`):**

```bash
"/Users/Shared/Epic Games/UE_5.6/Engine/Build/BatchFiles/Mac/Build.sh" \
  FirstLifeEditor Mac Development \
  -project="/Users/pedrohm/Documents/Pedro/codes/game/hunt/FirstLife.uproject"
```

**Pass criterion:** UBT prints **`Result: Succeeded`** (the same success line `docs/BUILD_LOG.md` recorded
for Slice 1). Any `Result: Failed`, or compile errors referencing the new Slice 2 files, is a hard stop.

**Slice 2 files that must compile clean** (from the contract's ownership lanes):
- **P2 lane:** `AnimalConfig.*` (reindeer config factory — data-as-code), `FirstLifeGameMode.*` (deferred
  spawn of 8–12 prey → `SetConfigOverride(reindeer)` → `FinishSpawning` → `SpawnDefaultController`).
- **A1 lane:** `AnimalAIController.*` + new brain/boids/state-machine classes.
- **Shared seam (already made — must NOT be re-touched):** `AAnimalCharacter::SetConfigOverride(...)`.

**Watch specifically for:** any diff that added a `UPROPERTY`, component, or field to `AAnimalCharacter`.
The contract forbids it ("If A1 or P2 believes it needs a component/field on `AAnimalCharacter`, STOP and
report to the Chief Architect"). If the build only succeeds because that file grew, that is a contract
breach to flag, not a pass.

---

## 3. Step 2 — Headless load check (map + reindeer config asset)

**Precedent exists.** `docs/BUILD_LOG.md` (2026-07-03 entry) records: *"Map + `DA_Human` verified via
headless PythonScript commandlet (exit 0, asset on disk)."* So a headless load-check via the PythonScript
commandlet **is** an established procedure for this project — reuse it for Slice 2.

**Important nuance for Slice 2:** the reindeer is a **runtime-built `UAnimalConfig` instance, not a
`.uasset`** (headless agents cannot author binary assets — `SLICE2_CONTRACT.md`). The GameMode builds the
reindeer config in code and injects it via `SetConfigOverride`. Therefore there is **no `DA_Reindeer.uasset`
on disk to load-check** the way `DA_Human` was. The headless check for Slice 2 is narrower: it can confirm
the **map loads and the C++ classes/module cook**, but it **cannot** confirm the herd spawns or behaves —
that requires PIE (Play In Editor), which is the H5 protocol's job.

### 3a. Minimal headless load-check (proposed — the smallest honest check)

Run the existing map-load Python via the commandlet in `-unattended -nullrhi` mode. The commandlet entry
point mirrors what Slice 1 used:

```bash
"/Users/Shared/Epic Games/UE_5.6/Engine/Binaries/Mac/UnrealEditor-Cmd" \
  "/Users/pedrohm/Documents/Pedro/codes/game/hunt/FirstLife.uproject" \
  -run=pythonscript \
  -script="/Users/pedrohm/Documents/Pedro/codes/game/hunt/Scripts/build_greybox_map.py" \
  -unattended -nullrhi -nosplash
```

**Pass criteria:**
- Process exits **0**.
- Log contains the script's own success/idempotency line (it logs *"GreyBox map already exists — loading it,
  not rebuilding"* on a clean tree, or the build lines on first run).
- No `LogPython: Error` / uncaught exception, no missing-module or missing-class errors for the Slice 2
  types (`AnimalConfig`, `AnimalAIController`, `FirstLifeGameMode`).

**What this proves:** the module loads, the map opens headless, and the Python API surface is intact —
i.e. nothing in the Slice 2 diffs broke asset registry / module startup. **What it does NOT prove:** herd
spawn count, boids behavior, gait selection, FID, or any H5 behavior. Do not overclaim it.

### 3b. Optional stronger check (propose to the Architect; QA does not author it)

If the Architect wants a headless assertion that the **reindeer config factory produces sane values** and
the **GameMode wires up 8–12 prey**, the honest way is a tiny Python probe that (a) constructs/loads the
reindeer `UAnimalConfig` via the same factory the GameMode uses and asserts `PreferredGait == Jog` (the
contract's load-bearing choice — see the defect list) and the three gait speeds are present, and (b) counts
spawned `AAnimalCharacter` actors carrying the reindeer config in a headless PIE (`-game -nullrhi`) frame.

QA proposes this but does **not** write it — it would touch code/scripts outside QA's `docs/qa/**` lane. It
is logged here so the Architect can decide whether the extra assertion is worth the authoring cost, or
whether PIE + the H5 protocol already covers it (likely: it does, more honestly, with a human in the loop).

---

## 4. Result recording

Record the run in this doc (QA may append results here — it's in the `docs/qa/**` lane) or hand the outcome
to the Architect for `docs/BUILD_LOG.md`:

| Check | Command | Expected | Result | Date |
|---|---|---|---|---|
| Editor target compiles | `Build.sh FirstLifeEditor Mac Development` | `Result: Succeeded` | ☐ | |
| Headless map/module load | `UnrealEditor-Cmd -run=pythonscript … -nullrhi` | exit 0, no errors | ☐ | |
| `AAnimalCharacter` untouched (no new field/component) | `git diff` on `AnimalCharacter.*` | seam-only, no additions | ☐ | |

A green build + green headless load is a **gate to open PIE**, not evidence about H5. Behavior lives in the
H5 playtest protocol.
