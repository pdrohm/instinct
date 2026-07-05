# CURRENT SPRINT — The First Life

> What we are doing *now* and why. Maintained by the Chief Architect. Every task names the hypothesis it
> moves (per `HYPOTHESES.md`); work that maps to no hypothesis is scope creep.
> Companion: `TASK_GRAPH.md` (dependencies), `PROJECT_STATUS.md` (state).
> Sprint opened: 2026-07-04. (Previous sprint "Unblock the gates, prep Slice 2" closed — its fan-out
> happened and overshot: the whole play-loop got built. See `IMPLEMENTATION_LOG.md` iters 1–6.)

---

## Sprint theme: **Validate, don't build**

The persistence-hunt vertical slice is code-complete: hunger → chase → contagious flush → catchable
straggler → exhaustion catch → feed → lose-and-track spoor, instrumented by the F1 telemetry overlay.
Every number in it is a first-pass guess and **no human has played a single minute of it**. The project's
own law ("validate before build") says the next unit of progress is a controller in someone's hands, not
another system. The autonomous dev loop was deliberately paused at iter 6 for exactly this reason.

**Hypotheses in scope:** H2 (endurance rhythm), H14 (persistence-hunt pacing — the make-or-break),
H5 (herd believability), H17 (perception reads). No Tier-2 hypothesis may grow this sprint.

---

## Owner tasks (the sprint IS these — everything else waits)

### 1. GATE-A — Play the build (~20 min) and rule on feel  ← THE bottleneck
Open `FirstLife.uproject` → Play. **WASD** jog · **Shift** sprint · **Ctrl** walk · **P** possession ·
**1–4** species perception · **F1** telemetry overlay.
Protocols: `docs/qa/H5_LIVING_HERD_PROTOCOL.md` (herd), then the hunt itself.
Rulings needed:
- **H2:** does run-by-default + scarce sprint create the rhythm? Tune `DA_Human` live.
- **H14:** pick the straggler out with F1, run it down. Is time-to-kill in a fun band (~3–8 min)?
  Does denying rest feel like *hunting* or like herding a slow cube?
- **H5:** does the contagious flush read as a herd deciding, or as scripted panic?
- Which first-pass numbers feel worst? (Log them — they seed the tuning pass.)

### 2. GATE-B — Sign off the perception architecture (unchanged, four decisions)
Recommended answers in brackets (from the audit):
1. Make `SPECIES_COGNITION.md` canonical, others superseded-in-framing. *[yes]*
2. Umwelt Slice-1 scope: Visual channel + fog grid, Absent/Tracked, no Memory. *[yes]*
3. Authorise the fog-render spike (post-process vs decal). *[yes, spike it]*
4. Build-order reorder: "AI reads the Umwelt" becomes the next-cheapest believability payoff. *[yes]*

### 3. Editor-side presentation work (parallel, optional this sprint, GUI-only)
- **Skeletal human** (GATE-C remainder): the imported human is a *static* mesh — it can never animate.
  Game Animation Sample / mannequin migration, ~15 min of Fab/Epic GUI work.
- **AnimBPs** (optional): C++ clip driver already animates deer/tiger/wolf; per-species ABPs upgrade it
  free when created (`docs/ANIMATION.md` has the exact recipe).
- **GATE-D — Tundra Stage 1**: sculpted Landscape + layered material + low sun/fog + hand scatter,
  a few hours, spec in `docs/ENVIRONMENT_TUNDRA.md`. Greybox suffices for GATE-A — do not block on this.

---

## The moment GATE-A clears — pre-planned fan-out (do NOT start early)

- **Feel-pass verdicts →** a *tuning* sprint, not a feature sprint: fold the owner's rulings back into
  the config factories (`AnimalConfig.cpp`), re-run the QA protocols, commit the first *validated* numbers.
- **GATE-B clears →** `perception-engineer` on **U1 (Umwelt extraction + Vision channel)**;
  `research-engineer` on **R2 (fog-render spike)** feeding **U2**. The herd brain's `SenseHerd()` seam is
  already marked for the Umwelt swap.
- **Only after both:** consider breadth again (predator pressure using the wolf/tiger validation pairs,
  day/night, weather). Nothing new before a validated core.

## Explicitly NOT this sprint (scope wall)

No new gameplay systems, no Umwelt code before sign-off, no combat/crafting/multiplayer, no netcode,
no renaming refactors touching the input seam (`AWolfPlayerController` rename stays queued), no PCG
(Stage 2 tundra) before Stage 1 validates. The dev loop stays paused until a human has played.
