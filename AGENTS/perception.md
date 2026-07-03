# Agent: perception-engineer

**Objective (one):** Build **Umwelt Slice 1** — extract a real per-agent belief store and make the Vision
channel write into it. **Gate:** GATE-B signed off. **Track:** U (node U1). **Hypothesis:** H17.

**Owns:** `SpeciesPerceptionComponent.*`, `SpeciesPerceptionProfile.*`, new `Umwelt`/belief-store class(es).
**May NOT touch:** locomotion, stamina, AI brains, HUD, `ScentFieldSubsystem` internals, `AAnimalCharacter`
wiring (request component additions through Integration).
**Depends on:** GATE-B decisions #1–#4. Consumes the audit's "Recommended Umwelt Slice 1".

**Definition of Done:** a standalone per-observer Umwelt object holding (a) an entity ledger with two rungs
(Absent/Tracked) + age, and (b) a coarse spatial "knownness" fog grid over the play area. The existing
vision gate (FOV × clarity range × LOS, no fabrication) writes Tracked entities into the ledger and marks
fog cells; static terrain always renders. No decay, no Memory, no Motion/Scent. Start-generous. Build green.

**Acceptance test:** With `firstlife.PerceptionDebug 1`, the belief store is inspectable and the player-body
hide/show is now driven *through* the Umwelt (not directly). Human (key 1) vs big cat (key 4) still feel
like different creatures — proving legibility-of-absence. The store is structured so an AI brain could read
it later (node U4) without touching the renderer.

**Ready-to-paste prompt:**
> [PASTE THE SHARED CONTEXT BLOCK FROM AGENTS/README.md]
> Your single objective: implement Umwelt Slice 1. Read `docs/SPECIES_COGNITION.md` §5 (canonical), the
> "Recommended Umwelt Slice 1" in the design-consistency audit summary in `CURRENT_SPRINT.md`, and the
> current `SpeciesPerceptionComponent.*` (it already does Absent/Tracked vision gating for the player body
> via `VisionChannelSees`/`UpdateKnowledge` using `HiddenTargets`+`SecondsSinceSeen`). Extract that embryonic
> belief state into a standalone **per-agent Umwelt object**: an entity ledger (per-entity rung {Absent,
> Tracked} + age) plus a coarse spatial knownness grid over the ~100×100 m map (tens-of-cm cells are fine).
> The Vision channel (generalise the existing gate) writes into it; the renderer reads from it to hide/show
> actors and (later) veil fog cells. Static terrain always renders — only dynamic entities are gated. NO
> decay, NO Memory rung, NO Motion/Scent channel, NO fog *rendering* (that is rendering-engineer's job — you
> expose the grid data). Keep it generous. Do not fabricate percepts. Do not touch locomotion/AI/HUD. If you
> need a component added to `AAnimalCharacter`, describe it and stop — Integration wires it. Build must
> succeed. Report what you changed, what H17 question it now enables, and what you deliberately deferred.
