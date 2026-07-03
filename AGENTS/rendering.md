# Agent: rendering-engineer

**Objective (one):** Render the Umwelt's fog grid as a legible "veil of absence" (node U2). **Gate:** GATE-B
#3 decided (post-process vs decal) + U1 exposes the grid. **Track:** U. **Hypothesis:** H17/H15 (legibility).

**Owns:** the fog renderer (post-process material or ground decal + its C++ driver reading the U1 grid),
per-species post-process grade in `SpeciesPerceptionComponent` (presentation only).
**May NOT touch:** belief-store logic (read the grid, never write it), gameplay, AI.
**Depends on:** `perception-engineer` U1 (the knownness grid data); `research-engineer` R2 (technique survey).

**Definition of Done:** unknown cells read as *"I don't know what's there"* — a soft desaturated veil, NOT
darkness and NOT a glitch (this is the make-or-break rendering risk, KNOWLEDGE_FOG §8). Known cells clear.
Driven entirely by the U1 grid via a render-target; no per-pixel sensing. Debug view (`firstlife.PerceptionDebug`)
still draws ground truth for us. Build green; holds frame budget at prototype scale.

**Acceptance test:** A tester describes veiled areas as *not-knowing*, not as night or a bug (KNOWLEDGE_FOG
§8 legibility-of-absence). Switching human↔big-cat visibly changes the fog shape.

**Ready-to-paste prompt:**
> [PASTE SHARED CONTEXT BLOCK]. Single objective: render the Umwelt knownness grid (node U2) using the
> approach chosen in GATE-B #3 (see `docs/research/` R2 survey). Read the grid data U1 exposes; draw a veil
> for unknown/veiled cells that reads as absence-of-information (desaturated, soft), never darkness. Update a
> render-target from the grid each frame; keep it cheap. Do not write to the belief store, do not touch
> gameplay/AI. Keep the debug ground-truth view. Build must succeed. Report legibility approach + budget.
