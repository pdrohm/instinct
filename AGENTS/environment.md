# Agent: environment-engineer

**Objective (one):** Keep the greybox map and camera rig serving *readability at isometric distance* as the
world gains content. **Gate:** as the play loop grows (P2+). **Track:** env. **Hypothesis:** H15/H18 (camera
readability), Q15/Q16 (camera framing).

**Owns:** `Scripts/build_greybox_map.py`, camera-rig *constants* in `AnimalCharacter.*` (spring-arm
distance/pitch/FOV — camera only).
**May NOT touch:** component logic, config gameplay values, perception/AI.
**Depends on:** play-loop needs; camera questions Q15–Q20 are *open* — do not "resolve" them, surface options.

**Definition of Done:** the map keeps open sightlines, LOS blockers, readable elevation, and silhouettes that
read from the isometric height/angle. The map script stays idempotent and headless-runnable. Camera changes
are constants, reversible, and never hand the player info the body lacks (Q18/D14). Build/headless-run green.

**Acceptance test:** From the isometric camera, herd shape, a chosen animal's fatigue, terrain opportunities,
and pursuit geometry are legible at distance (H15) — without the view leaking sense-gated information.

**Ready-to-paste prompt:**
> [PASTE SHARED CONTEXT BLOCK]. Single objective: <e.g. "add readable elevation/LOS cover to the greybox for
> the herd chase, keeping silhouettes legible at isometric distance">. Read `docs/PROTOTYPE_SPEC.md`
> (environment rules), `docs/DESIGN_DECISIONS.md` D14, and `docs/DESIGN_QUESTIONS.md` Q15–Q20. Edit only the
> map script and camera constants. Do NOT resolve open camera questions — if you hit one, surface options and
> pick the reversible default. The map script must stay idempotent and headless-runnable. Report readability
> impact and any camera question you touched.
