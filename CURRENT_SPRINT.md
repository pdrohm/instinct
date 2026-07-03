# CURRENT SPRINT — The First Life

> What we are doing *now* and why. Maintained by the Chief Architect. Every task names the hypothesis it
> moves (per `HYPOTHESES.md`); work that maps to no hypothesis is scope creep.
> Companion: `TASK_GRAPH.md` (dependencies), `PROJECT_STATUS.md` (state).
> Sprint opened: 2026-07-03.

---

## Sprint theme: **Unblock the gates, prep Slice 2**

The prototype is authored but unvalidated, and the biggest downstream track (perception/cognition) is
design-locked. This sprint's job is **not** to write play-loop code — it is to clear the three gates so the
next sprint can fan out implementation safely, and to pre-stage everything that clearing them will need.

**Hypotheses in scope:** H2 (feel-pass), H5 (herd prep), H14 (persistence-hunt prep), H17 (perception
sign-off). No Tier-2 hypothesis may grow this sprint.

---

## Owner decisions required (these are the sprint's real deliverable to *you*)

The Chief Architect cannot proceed past these without the vision-holder. They are presented so one sitting
resolves them.

### GATE-A — Play the build (10 min) and rule on feel
Play the grey-box (`FirstLife.uproject` → Play; WASD jog, Shift sprint, Ctrl walk, P release, 1–4 species).
Ruling needed: does run-by-default + scarce sprint create the H2 rhythm? Tune `DA_Human` live if not.
*This is the gate to the entire play-loop track (P2–P5).*

### GATE-B — Sign off the perception architecture (four coupled decisions)
The four perception docs converge; they need one blessing. Recommended answers in brackets (from the audit):
1. **Canonical spec:** make `SPECIES_COGNITION.md` canonical, mark the other three superseded-in-framing. *[recommend: yes]*
2. **Umwelt Slice-1 scope:** Visual channel + fog grid only, Absent/Tracked, no Memory. *[recommend: yes — proves legibility-of-absence with least code]*
3. **Fog rendering:** authorise a short spike to choose post-process material vs. ground decal. *[recommend: yes, spike it]*
4. **Build-order reorder:** "AI reads the Umwelt" becomes step 3 (before Motion/Scent). *[recommend: yes — cheapest believability payoff]*
   *Deferrable (no decision needed now): two-decay model, generosity dial (recommend start-generous), wind model, attention postures.*

### GATE-C — Acquire the human mesh
~15 min of Fab/Epic GUI work: Game Animation Sample (free, Manny-native, maps 1:1 to Walk/Jog/Sprint) +
a mannequin/tribal mesh; point `AAnimalCharacter`'s mesh at it. Unblocks presence-quality playtests (H3/H18).
*Binary asset — cannot be delegated to a headless agent.*

---

## Work delegated / done this sprint (parallel, non-code, safe)

| Task | Agent | Output | Status |
|---|---|---|---|
| Code architecture map (ground truth) | Explore | internal map → `PROJECT_STATUS.md` | ✅ done |
| Perception design-consistency audit | general-purpose | GATE-B blocker list (above) | ✅ done |
| Herd + prey research brief (R1) | research-engineer | `docs/research/HERD_AND_PREY.md` | ✅ done |
| Remove checked-in secret (X0) | Chief Architect | `Config/DefaultEngine.ini` | ✅ done |
| Governance docs (X2) | Chief Architect | `PROJECT_STATUS`/`TASK_GRAPH`/`ARCHITECTURE_DECISIONS`/`CURRENT_SPRINT`/`AGENTS/` | ✅ done |

**Key research result (feeds next sprint, not this one):** prey species = **reindeer/caribou**, unambiguously
— reads more legibly as a flushing herd at isometric distance, ships free (ANIMAL VARIETY PACK), and its
endurance profile keeps the persistence-hunt asymmetry honest (the horse's own endurance would muddy it).
Herd model: many-eyes as two separable knobs (detection ↑ with size, nervousness ↓ with dilution),
*contagious* flush (neighbour-to-neighbour, not a global panic flag), FID as a decision surface. Full
sourced tuning table in the brief. **Do not build against this yet — it is staged for post-GATE-A.**

---

## The moment a gate clears — pre-planned fan-out (do NOT start early)

- **GATE-A clears →** spawn `gameplay-locomotion-engineer` + `ai-engineer` on **P2 (herd) / A1 (herd brain)**
  in parallel, using `docs/research/HERD_AND_PREY.md`. Then P3 → P4 (the H14 crux).
- **GATE-B clears →** spawn `perception-engineer` on **U1 (Umwelt extraction + Vision channel)**;
  `research-engineer` on **R2 (fog-render spike survey)** feeding `rendering-engineer` on **U2**.
- **GATE-C clears →** `asset-pipeline-engineer` integrates the mesh; re-run presence playtests (H3/H18).

Ready-to-paste prompts for each are in `AGENTS/`.

## Explicitly NOT this sprint (scope wall)

No prey/herd code, no hunger, no scent gameplay, no Umwelt code, no combat/crafting/multiplayer, no AI
brain beyond the wander stub, no renaming refactors touching the input seam. All wait on a gate.
