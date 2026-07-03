# H5 Playtest Protocol — "The Living Herd" (Slice 2)

> **Owner:** QA. **Status:** ready to run once the Chief Architect confirms a green integrated build
> and the reindeer herd is spawning. **Read before running:** `docs/HYPOTHESES.md` (H5, and H14/H15 for
> context), `docs/PROTOTYPE_SPEC.md` (Success Criteria #3 and #5), `SLICE2_CONTRACT.md` (the gait mapping —
> this is where the most likely bug lives).
>
> **The hypothesis under test (H5):** *"A living herd (not a target) makes the world feel alive."* More
> precisely — does an autonomous, believably-behaving reindeer herd make a naive player perceive the world
> as **alive and indifferent** (an entity with its own agenda), rather than as **targets placed for them**?
>
> **This protocol is falsifiable by design.** Every phase names an explicit PASS signal and an explicit FAIL
> signal. If the run produces FAIL signals, that is a *valid and valuable result* — it saves us building
> Slice 3–5 on a broken pillar. Do not rationalize a weak herd into a pass.

---

## 0. What "done" means for Slice 2 (the bar we are measuring against)

From the Chief Architect's Definition of Done for this slice:

1. The herd **grazes/drifts as a cohesive living group with ZERO player input** and reads as alive from the
   isometric camera.
2. Crossing a prey's **FID** (flight-initiation distance) **propagates alarm neighbor-to-neighbor** and the
   group flees — as a *wave*, not as N independent bolts.
3. The flee leaves an **occasional straggler** (emergent, from per-agent nervousness/reaction jitter — not a
   scripted "slow one").
4. When pressure stops, the herd **calms and returns to grazing**.
5. All of the above is produced with **no per-species subclass** and **all tuning in data** — a QA concern
   only insofar as behavior that *can only* be fixed by code (not data) is a design-debt flag to log.

Slice 2 does **not** yet include prey stamina, the feed, or hunger (those are steps 4–5). So a "pursuit"
here is only *"press one animal and watch the herd's social response"* — you cannot yet catch or feed.

---

## 1. Setup (a naive human can follow this verbatim)

1. Confirm with the Chief Architect that the current integrated build is green and the reindeer herd spawns
   (8–12 animals). **Do not build it yourself** — QA holds the build.
2. Launch the editor and open the project:
   `"/Users/Shared/Epic Games/UE_5.6/Engine/Binaries/Mac/UnrealEditor" "<repo>/FirstLife.uproject"`
   (or double-click `FirstLife.uproject`).
3. If the `GreyBox` map is not already the loaded level, run `Scripts/build_greybox_map.py` via
   **Tools → Execute Python Script** (idempotent — it loads the existing map, does not rebuild).
4. Press **Play In Editor (PIE)**. You possess the human on the open steppe, isometric camera.

### Controls (verified against `WolfPlayerController.cpp`)

| Input | Effect | Notes |
|---|---|---|
| **W A S D** | Move (jog) | Screen-relative to the fixed isometric camera. W is always "up" on screen. Default gait = jog. |
| **Left Shift** (hold) | Sprint | Burns stamina; dies mid-stride at exhaustion. |
| **Left Ctrl** (hold) | Walk | Slow gait; stamina recovers faster. |
| **P** | Release / reclaim the body to/from its AI brain | Player spectates while AI drives; camera stays on the body. Use this to watch the human's *own* resident brain. |
| **1 / 2 / 3 / 4** | Perception-readout debug switch (1 human, 2 deer, 3 wolf, 4 big cat) | **This does NOT change your body or the herd.** It only re-skins the HUD perception readout. Ignore it for H5 unless told otherwise; note it so the tester isn't confused. |

**Tester instruction script (read aloud, then say nothing):** *"You are an early human on the steppe. There
are no goals, no objective, no HUD quest. Just exist here for a few minutes and do whatever you feel like.
Think out loud — say what you notice and what you're thinking, even if it feels trivial. I won't answer
questions during the session."*

**Observer's job:** stay silent. Record verbatim quotes with timestamps, and note *where the tester's
attention goes* (do they look at the herd? approach it? watch it?). The unprompted narration is the primary
data; body language and dwell time are secondary data.

---

## 2. Phase A — Do-Nothing Observation (60 seconds, zero input)

**Setup:** Tell the tester (or, for a solo QA dry-run, yourself) to **put the controller/keyboard down for a
full 60 seconds** and just watch the herd. No movement, no input at all.

**What we are testing:** does the herd read as *alive and idle* with the player passive — or as *frozen /
particle-like / on rails*? This is the purest read on DoD #1 and Success Criterion #3 (world is alive on its
own, player is non-central).

**Watch for:**
- Does the group *drift and reshape* (meander, re-cluster, individual graze-pauses and head movement)?
- Does it stay *cohesive* (reads as one herd) without being *rigid* (marching-band alignment)?
- Do individuals do slightly different things at slightly different times (heterogeneity), or do they move
  in lockstep?
- Does the herd stay on the map, grazing an area — or drift toward / into a wall?

**PASS signal:** Within 60 s the herd visibly *lives* — it drifts, reshapes, individuals pause and resume,
the cluster loosens and tightens — and the tester narrates it as ongoing behavior ("they're just grazing
over there," "that one wandered off a bit," "they keep drifting left"). The tester's attention is *drawn to*
the herd without being told to look.

**FAIL signals (any one is a fail for this phase):**
- The herd is **static** — animals stand still or vibrate in place; it reads as frozen furniture.
- The herd moves as a **rigid blob** — all animals identical heading/speed, "particle system" look, no
  individual variation. (Root cause to suspect: no per-agent jitter, or alignment weight too high in the
  graze profile.)
- The herd **cruises continuously at jog speed** while "grazing" — reads as perpetually fleeing nothing.
  **This is the single highest-probability Slice-2 bug** (see `SLICE2_CONTRACT.md`: if the brain forgets to
  assert `SetWantsToWalk(true)` as its default, the reindeer's `PreferredGait = Jog` makes an idle herd
  cruise at 520 cm/s). If you see the herd "grazing" at a fast, purposeful clip, log it as **DEFECT — herd
  cruising at Jog** and stop trusting the graze read until it's fixed.
- The herd **drifts off the map / into a wall** and piles up (no home/resource bias).
- The tester ignores the herd entirely or reads it as scenery ("is that just decoration?").

---

## 3. Phase B — Approach (does FID behave as a decision surface?)

**Setup:** Now the tester moves. Ask them to *approach the herd* — but run this phase **four ways** (do them
in separate approaches, letting the herd recover to grazing between each; see Phase D for recovery):

1. **Direct + fast** — sprint straight at the herd.
2. **Direct + slow** — walk (Ctrl) straight at the herd.
3. **Oblique + slow** — walk on a tangent, angling closer gradually rather than head-on.
4. **Oblique + fast** — jog on a tangent.

**What we are testing:** is **flight-initiation distance a *decision surface*** — a function of approach
angle and speed and herd state — rather than a fixed radius trip-wire? And when the herd *does* flush, does
alarm **propagate as a wave** (one animal reacts, neighbors copy within a fraction of a second) or do all
animals **bolt independently and simultaneously** the instant the player crosses one radius? This is DoD #2
and Success Criterion #5 (reading the situation and using the read to decide).

**Watch for:**
- Does a **fast, direct** approach flush the herd at a *noticeably greater distance* than a **slow, oblique**
  one? (The research target: oblique/slow ≈ 0.5× the direct/fast FID — the player should be able to *get
  meaningfully closer* by approaching carefully.)
- At the moment of flush, is there a **visible ripple** — the nearest animal reacts first, then its
  neighbors, then the far side — or does the whole herd snap to fleeing on the same frame?
- Do the animals flee in a **coordinated, polarized direction** (everyone points roughly the same way, herd
  bunches) or scatter in all directions like startled pigeons?
- Does the tester *notice and exploit* the difference — e.g. "if I creep up sideways I can get closer"?

**PASS signals (need the first two; the third is the strong signal):**
- The herd **flushes earlier** to fast/direct approaches and **tolerates a closer** slow/oblique approach —
  a repeatable difference, not noise.
- The flush reads as a **contagious wave** across the herd, and the flee is **directional and coordinated**
  (polarized away from the player, bunched).
- **Strong pass:** the tester *unprompted* discovers approach as a skill — narrates a plan to close distance
  ("I'll come at them from the side," "if I don't sprint they let me get closer") and treats the herd as
  something to be *read*, not walked into.

**FAIL signals:**
- FID is a **hard constant** — the herd flushes at the exact same distance regardless of angle or speed
  (trip-wire, not decision surface). The "read the situation" skill has nothing to bite on.
- The flush reads as a **global flag** — every animal starts fleeing on the same frame, no wave, no
  nearest-first ordering. (Root cause to suspect: a herd-wide "panic" boolean instead of per-agent alarm
  propagation.)
- The herd **scatters incoherently** (each animal a random vector) instead of fleeing together — reads as N
  panicked particles, not a herd.
- The herd **flees directly away in a straight line forever** with no cornering, no terrain use, pulling
  cleanly away at a constant gap — reads as a fleeing conveyor belt, not animals.
- The tester treats the flush as arbitrary ("they just teleported into run mode") and stops trying to read
  it.

---

## 4. Phase C — Pursuit / Pressure (does a straggler emerge? does the herd split?)

**Setup:** Ask the tester to *pick one animal and keep pressure on it* — chase it, cut toward it, keep it
moving. (Reminder: in Slice 2 there is **no feed and no prey stamina yet**, so nothing can be caught. We are
only testing the herd's *social* response to sustained directional pressure.)

**What we are testing:** under sustained pressure, does an **occasional straggler emerge** (an animal that
lags — from per-agent nervousness/reaction jitter, not a script) and does the **herd fragment** (a pressed
individual gets cut from the group)? This is DoD #3 and the emotional seed of the eventual persistence hunt
(H14) — the moment the herd stops being a single blob and becomes individuals.

**Watch for:**
- Does one animal **lag behind** the main group under pressure — and is it a *different* animal on different
  runs (emergent), or always the same one (scripted / a fixed slow config)?
- Does the herd **split** — does the pressed individual (or a small clump) get separated from the main body
  and run its own line?
- Does the tester's language shift from *"the herd"* to *"that one"* — do they start tracking an individual?

**PASS signals:**
- A **straggler emerges** under pressure — and across 2–3 runs it is **not always the same individual**
  (evidence it's emergent jitter, per the contract's "reaction/nervousness jitter is enough").
- The herd **visibly fragments** under directional pressure — a pressed animal separates from the group.
- The tester **fixates on an individual** ("come on, you, you're falling behind") — the herd has resolved
  into animals with different fates. This is the strongest H5 signal available in Slice 2.

**FAIL signals:**
- **No straggler ever** — the herd stays a perfect cohesive blob under any pressure; every animal keeps
  identical spacing forever. (Root cause: heterogeneity not implemented, or jitter too small to matter.)
- **The same animal always lags** on every run — reads as a scripted "designated victim," which is *worse*
  than no straggler because it exposes the seams. (Flag as a design-honesty defect: the straggler must be
  emergent.)
- The herd **never splits** — cohesion is unbreakable, so the group can never be worked apart. (Root cause:
  no cohesion-break / split threshold; the persistence-hunt core loop is un-buildable on top of this.)
- The tester keeps saying *"the herd"* and never tracks an individual — the group never resolves into
  animals.

---

## 5. Phase D — Recovery (do they calm and return to grazing?)

**Setup:** Ask the tester to **stop pressing** — back off entirely, put input down, and watch for 30–60 s.

**What we are testing:** DoD #4 and the spec's explicit requirement — prey that escape far enough or are no
longer pressed **cool down, re-aggregate, and return to grazing**. Without this, the world reads as
permanently spooked, and the "indifferent living world" (it doesn't revolve around you) collapses.

**Watch for:**
- Does the alarm **decay over time** — do animals visibly de-escalate from flee-cruise back to walk/graze?
- Does a **split-off straggler re-join** the main herd, or does it stay isolated forever?
- Does the herd **re-form and resume idle drift** (back to the Phase A "alive and idle" state)?
- Roughly how long does recovery take? (Research first-pass guess: ~8–15 s of no pressure. Log the actual
  felt duration — too fast reads robotic, too slow reads like a grudge.)

**PASS signal:** Within a reasonable window (~10–20 s of no pressure) the herd **visibly calms** — slows to
walk, stops fleeing, re-clusters, and resumes grazing/idle drift; a straggler rejoins. The world "moves on"
and stops centering the player.

**FAIL signals:**
- The herd **stays in flee state indefinitely** — keeps cruising/running long after the player has stopped
  and left. (Root cause: alarm scalar not decaying, or decay far too slow.)
- The herd **snaps** instantly from full flee to full graze with no transition — reads as a state-machine
  flip, not an animal calming down.
- A split straggler **never rejoins** — the herd permanently loses members every time it's pressed.
- Recovery is so fast the flush had no weight ("oh they don't care") — undercuts the tension the whole loop
  needs.

---

## 6. HIGHEST-VALUE OUTPUT — "Felt unfair / felt like a bug" log

> This section is the single most valuable thing this playtest produces. A herd can pass every phase above
> and still *feel wrong* in ways that only surface as a tester's gut reaction. Capture every such moment with
> **exact-repro instructions**, because a felt-wrong moment we can reproduce is a fixable moment. Log it even
> if you're not sure it's a bug.

For each moment, record:

| Field | What to write |
|---|---|
| **Timestamp** | PIE session time (e.g. `02:14`). |
| **Tester quote (verbatim)** | The exact words. "That's bullshit," "wait, why did it do that," "that felt fake." |
| **What the tester was doing** | Approach angle + gait (direct/oblique, walk/jog/sprint), distance to herd, which animal, terrain nearby. |
| **What the herd did** | The observed behavior that triggered the reaction. |
| **Repro steps** | The minimal sequence to make it happen again — precise enough for an engineer to reproduce headless-ish or in PIE. E.g. *"Sprint straight at herd from the east rock at ~15 m → entire herd flushes on one frame with no wave."* |
| **Felt-unfair vs. felt-like-a-bug** | Unfair = the *design* feels wrong (e.g. "they always outrun me even when I sneak"). Bug = it looks *broken* (e.g. an animal moonwalks, clips a wall, freezes). Different owners: unfair → tuning/design; bug → code. |
| **Suspected root cause** | Optional, if obvious (e.g. "global panic flag," "herd cruising at jog," "no straggler jitter"). |

**Prompt the tester once, at the very end** (never mid-session): *"Was there any moment the herd did
something that felt unfair, or felt fake or broken? Walk me back to it."* Then reproduce it together and fill
the table.

---

## 7. Verdict rubric — reaching a clear yes/no on H5

Score the session against the DoD, using the phase results:

| H5 sub-claim | Evidence source | Result |
|---|---|---|
| Herd reads as **alive & idle** with zero input | Phase A PASS | ☐ Yes ☐ No |
| Herd is **cohesive but not rigid** (no particle look, no jog-cruise) | Phase A | ☐ Yes ☐ No |
| **FID is a decision surface** (angle/speed matter) | Phase B | ☐ Yes ☐ No |
| Flush **propagates as a wave**, flee is **coordinated** | Phase B | ☐ Yes ☐ No |
| **Straggler emerges** and is **not always the same animal** | Phase C | ☐ Yes ☐ No |
| Herd **splits** under sustained pressure | Phase C | ☐ Yes ☐ No |
| Herd **calms and returns to grazing** on release | Phase D | ☐ Yes ☐ No |
| Tester **treats the herd as an entity with its own agenda** (unprompted) | All phases | ☐ Yes ☐ No |

**H5 = confident YES** if the last row is Yes *and* at least Phases A, B, and D pass — the tester spent time
observing before acting and described the herd as having its own behavior/agenda (Success Criterion #3),
independent of whether Slice 2's straggler is perfect yet.

**H5 = confident NO** if the herd reads as a dumb target: static/particle-like in Phase A, or a global-flag
flush in Phase B, and the tester talks about "spawns" / "targets" / "decoration" rather than a living group.
Per the spec, a herd that "flees stupidly breaks the living-world promise instantly" — that is a real,
saving result.

**H5 = "YES IF"** (the most likely outcome) if the herd is alive but one specific behavior betrays it (most
commonly: jog-cruise grazing, or a global-flag flush with no wave). Name the exact fix; it's a tuning/logic
correction, not a rejected hypothesis.

**Guardrail against fooling ourselves:** watch for *self-directed* observation and *unprompted* herd-as-agent
language — not polite compliance. A tester who dutifully "watches the herd because I was told to" is not
evidence. A tester who *chooses* to stop and read it is. (This mirrors the H1/H5 risk note in
`HYPOTHESES.md`: don't mistake compliance for engagement.)

**Sample size:** run **at least 3 naive testers**. H5's illusion is fragile and personal; one tester's read
is an anecdote, three converging reads is a signal.
