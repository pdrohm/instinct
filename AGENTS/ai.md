# Agent: ai-engineer

**Objective (one, per spawn):** Replace the wander stub with a real prey brain. First spawn = **A1, herd
state machine** (graze / alert / flee / return). **Gate:** GATE-A + P2 shape exists. **Track:** A.
**Hypothesis:** H5 (believable AI = the product), then H14 at A2.

**Owns:** `AnimalAIController.*` and new brain/state-machine/boids classes.
**May NOT touch:** perception internals, locomotion/stamina/config, HUD, scent field.
**Depends on:** `docs/research/HERD_AND_PREY.md` (boids weights, FID as a decision surface, contagious
flush, straggler heterogeneity). Later, node U4 (AI reads the Umwelt) needs `perception-engineer`'s U1.

**Definition of Done (A1):** a per-agent herd brain driving prey via the intent API
(`AddMovementInput` + `SetWantsToSprint`/`SetWantsToWalk`) — never by teleport or direct velocity. States:
graze (weak-alignment boids + wander), alert, flee (alignment spikes + predator-directed vector), return.
Flush is **contagious** (neighbour-to-neighbour), not a global flag. Per-agent nervousness heterogeneity so a
straggler emerges naturally. Runs identically whether AI- or player-driven (it's a brain, not a pawn hack).
Build green.

**Acceptance test:** With no player action the herd grazes cohesively; when the player crosses a prey's FID,
alarm propagates through neighbours and the group flees as a unit, leaving an occasional straggler. The brain
reads only from what it can sense (for now, proximity; later, the Umwelt via U4) — no omniscient targeting.

**Ready-to-paste prompt:**
> [PASTE THE SHARED CONTEXT BLOCK FROM AGENTS/README.md]
> Your single objective: build the prey herd brain (node A1, H5). Read `docs/research/HERD_AND_PREY.md`
> (boids substrate: one Reynolds core, two weight profiles blended by a per-agent alarm scalar; biology
> overrides in four places — predator-directed flight vector, per-agent heterogeneity → straggler, breakable
> cohesion → herd splitting, panic decay/return-to-graze) and `docs/PROTOTYPE_SPEC.md`. Replace/extend
> `AAnimalAIController`'s random wander with a graze/alert/flee/return state machine driving the prey via the
> existing intent API only (`AddMovementInput`, `SetWantsToSprint/Walk`) — the possession seam must stay
> intact (a player could take the body and it still works). Model FID as a decision surface (approach angle ×
> speed × herd size × prior harassment), make the flush contagious (neighbour-to-neighbour), and give each
> agent a nervousness scalar so a straggler emerges. Tunable constants in a data asset or config where
> possible. Do NOT read world-truth omnisciently — sense by proximity for now; a later task (U4) will feed
> you the Umwelt belief store. Do not touch locomotion/config/perception/scent. Build must succeed. Report
> what you built, that it moves H5, and the interface you expect from U4.
