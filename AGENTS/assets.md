# Agent: asset-pipeline-engineer  🔒 (human-in-loop for binary assets)

**Objective (one):** Acquire and wire bought/free assets per `docs/ASSET_BIBLE.md`, gated by validated
slices (D9/D10). **Gate:** GATE-C (mesh) and per-slice acquisition order. **Track:** assets.

**Owns:** mesh/anim *wiring* on `AAnimalCharacter` (SkeletalMesh components + retarget hookup), `Content/`
acquisition notes. **May NOT touch:** gameplay/perception/AI logic, config tuning values.
**Reality:** binary assets (`.uasset`, meshes, AnimBPs) cannot be authored as text — the *acquisition and
import* steps are **GUI work for the human**. This agent prepares the wiring code/notes and verifies the
result; it cannot download from Fab or import a mesh headlessly.

**Definition of Done (GATE-C, human mesh):** the free Game Animation Sample (Manny-native, maps 1:1 to
Walk/Jog/Sprint) + a mannequin/tribal mesh replace the grey-box cubes on `AAnimalCharacter`; gaits animate;
config/tuning unchanged (proves the H7 "cheap species swap" claim). Build green.

**Acceptance test:** The human reads as an animal body at isometric distance (unblocks H3/H18 presence
judging). Swapping the mesh required mesh + retarget only — no core rewrite (H7 evidence).

**Ready-to-paste prompt (for the wiring half; flag the GUI half to the human):**
> [PASTE SHARED CONTEXT BLOCK]. Single objective: wire an acquired SkeletalMesh + AnimBP onto
> `AAnimalCharacter`, replacing the two grey-box cube components, and retarget standard Walk/Jog/Sprint
> locomotion to it. Read `docs/ASSET_BIBLE.md` (standing rules: humans on the Epic Manny skeleton forever)
> and `IMPLEMENTATION_LOG.md` (the recommended acquisition path). Change ONLY mesh wiring on the character —
> no gameplay/config logic. If the mesh/AnimBP assets are not present in `Content/`, STOP and hand the human
> the exact Fab/migrate steps to import them; do not fake it. Build must succeed. Report H7 evidence (was it
> mesh + retarget only?).
