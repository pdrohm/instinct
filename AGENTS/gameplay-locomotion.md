# Agent: gameplay-locomotion-engineer

**Objective (one, per spawn):** Advance the play loop one build-order step. First spawn = **P2, the living
herd**. **Gate:** GATE-A passed (player economy validated). **Track:** P. **Hypothesis:** H5 (then H14 at P4).

**Owns:** `LocomotionComponent.*`, `StaminaComponent.*`, `AnimalConfig.*`, `LocomotionTypes.h`,
`WolfPlayerController.*`, `FirstLifeHUD.*`, `FirstLifeGameMode.*`, `Scripts/build_greybox_map.py`.
**May NOT touch:** perception, AI brain internals (coordinate the herd *brain* with `ai-engineer`),
`ScentFieldSubsystem`.
**Depends on:** GATE-A ruling + `docs/research/HERD_AND_PREY.md` tuning table. For P2's herd behaviour,
the AI brain (A1) is owned by `ai-engineer` — this agent owns the *prey `UAnimalConfig`* (a reindeer
locomotion/stamina profile) and the spawn/wiring; `ai-engineer` owns the boids/flee brain.

**Definition of Done (P2):** a reindeer prey `UAnimalConfig` exists (from the research tuning table, flagged
as guesses); the GameMode spawns a grazing herd of ~8–12 on the greybox (not the 3 debug targets); the herd
reads as a living group at isometric distance. Prey use the same three-gait locomotion component, data-only.
Build green.

**Acceptance test:** Play: the herd grazes and drifts as a cohesive group with no player interaction, and
reads as *alive on its own* (H5) from the isometric camera. Prey are pure `AAnimalCharacter` + config — no
new pawn subclass (ADR-E4).

**Ready-to-paste prompt:**
> [PASTE THE SHARED CONTEXT BLOCK FROM AGENTS/README.md]
> Your single objective: implement build-order step 2 (the living herd, H5). Read `docs/PROTOTYPE_SPEC.md`
> ("The world around you"), `docs/LOCOMOTION.md`, and `docs/research/HERD_AND_PREY.md` (recommended prey =
> reindeer; tuning table of first-pass values). Create a reindeer prey `UAnimalConfig` using the research
> table's locomotion/stamina values (prey escape-jog > human jog > prey recovery-walk; prey sprint > human
> sprint but empties in ~7 s with near-zero recovery — verify the three load-bearing speed numbers). Make
> `AFirstLifeGameMode` spawn a herd of ~8–12 reindeer `AAnimalCharacter`s (replace the 3 debug wander
> targets). Each prey is one `AAnimalCharacter` + the reindeer config — NO subclass. The herd's grazing/flee
> *brain* is owned by `ai-engineer` (node A1) — coordinate the interface (intent setters) but do not write
> the boids/state-machine yourself; if A1 isn't ready, give prey the existing wander brain as a placeholder
> and say so. Do not touch perception or scent. All tuning in the config asset. Build must succeed. Report
> what you changed, that it moves H5, and what you left for `ai-engineer`.
