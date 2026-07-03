# Agent: simulation-engineer

**Objective (one):** Generalise the scent placeholder into the species-agnostic **Stimulus Field** — the
world's decaying memory (node S1, ADR-E6). **Gate:** GATE-B signed off + scent nears (build step 5 / U6).
**Track:** S. **Hypothesis:** underpins the signature "the world remembers" mechanic.

**Owns:** `ScentFieldSubsystem.*` (→ Stimulus Field). **May NOT touch:** perception belief store, AI,
gameplay, rendering.
**Depends on:** GATE-B (defines the Stimulus-Field layer); `research-engineer` R3 (scent/diffusion model).

**Definition of Done:** append-and-decay records with `{position, features, timestamp, decay rate}` for
multiple stimulus kinds (start: scent + motion pulse), species-agnostic, wind-reshaped, bounded. Two
independent decays honored (field fade is separate from belief-confidence fade — that's the Umwelt's job).
Keeps the current placeholder's cheapness; NO per-blade/per-pixel. Build green.

**Acceptance test:** The field is a clean stimulus *source* a channel can sample by age (vision age≈0, scent
age minutes–hours). It writes nothing to any agent's belief — consumers sample it. Existing scent-overlay
debug still works.

**Ready-to-paste prompt:**
> [PASTE SHARED CONTEXT BLOCK]. Single objective: generalise `UScentFieldSubsystem` (today a flat FIFO
> `TArray<FScentPoint>` of loc+time with fixed-wind drift, ~75 s age-out — "FAKE on purpose") into the
> Stimulus Field per `docs/SPECIES_COGNITION.md` §5 layer ① and ADR-E6: append-and-decay records carrying
> position, feature-set, timestamp, and decay rate, for more than one stimulus kind, species-agnostic,
> wind-reshaped, bounded and cheap. Read `docs/research/` R3 for the model. It is a *source*: it must not
> write into any agent's belief store — channels sample it. Keep decay independent of belief decay. Do not
> touch perception/AI/gameplay/rendering. Build must succeed. Report the record schema and what samples it.
