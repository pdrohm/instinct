# Hypotheses — The Living Research Log

> The most important document in the repository, because it is the only one that admits what we don't know.
> The other docs (`GAME_DNA.md`, `DESIGN_PRINCIPLES.md`, `PLAYER_EXPERIENCE.md`, `DESIGN_DECISIONS.md`, `DESIGN_QUESTIONS.md`) are **decision filters**, not scripture. This document is the reason they can stay filters: it holds our beliefs as bets to be tested, not truths to be defended.
> The project is **learning-driven, not feature-driven.** Every prototype, sprint, and milestone exists to move one or more of these hypotheses from Unknown toward Validated or Rejected.

## How this document works

Each hypothesis is a claim we currently *believe* but have not *proven*. It carries a confidence level and a status, and it names how we intend to find out. When we learn something — from a playtest, telemetry, observation, or just building the thing — we update the entry. We never quietly delete a rejected hypothesis; a rejected bet is one of the most valuable things we can own, because it stops us re-litigating it.

**The rule:** no feature enters the build to "make the game better." It enters to *test a hypothesis*. If you cannot name the hypothesis a feature validates, the feature waits.

**Fields per hypothesis:**
- **Question** — the claim, phrased as something that can be proven false.
- **Why it matters** — what depends on the answer.
- **Confidence** — Unknown / Low / Medium / High (our honest current belief, not a hope).
- **Validation** — how we intend to actually find out.
- **Success looks like** — the observable signal that confirms it.
- **Risks** — how this could go wrong, or how we could fool ourselves.
- **Status** — Open / Testing / Validated / Rejected.
- **Prototype coverage** — does "The First Life" prototype (`PROTOTYPE_SPEC.md`) test this? (Yes / Partial / No — deferred). This tag is a scope guardrail: a "No" hypothesis must **not** be allowed to grow the current prototype.

---

## Tier 1 — Hypotheses the current prototype exists to answer

These are the bets "The First Life" is built to test. If we are spending prototype effort, it should be here.

### H1 — Living as an animal is inherently engaging without explicit objectives

**Question:** Will players stay engaged inhabiting an animal driven only by internal needs (hunger, stamina, awareness), with no quests, markers, or authored goals?
**Why it matters:** This is the load-bearing assumption of the entire project. If it's false, nothing downstream matters. Everything in `GAME_DNA.md` rests here.
**Confidence:** Medium. Precedent exists (survival and creature games), but "no objectives at all" is a real gamble.
**Validation:** Naive players play the grey-box prototype for ~10 minutes with zero instruction; we observe whether they self-direct and stay engaged.
**Success looks like:** Players invent their own goals ("I want to catch that slow one"), keep playing past the point of novelty, and describe intent unprompted.
**Risks:** Players stall, wait to be told what to do, or disengage. Also: we mistake polite compliance for genuine engagement — watch for *self-directed* behavior, not just continued play.
**Status:** Open.
**Prototype coverage:** Yes — this is the prototype's core question.

### H2 — Simple survival systems create interesting decisions, not busywork

**Question:** Does the hunger/stamina/awareness triangle produce genuine "is this worth it?" decisions rather than a chore of topping off bars?
**Why it matters:** `DESIGN_PRINCIPLES.md` #3. If survival systems read as busywork at the simplest scale, added complexity won't rescue them.
**Confidence:** Medium.
**Validation:** Observe whether players deliberate before acting, sometimes *choose not to* chase, and can articulate the tradeoff afterward.
**Success looks like:** Players refrain from a bad chase and feel good about it ("restraint as mastery," `PLAYER_EXPERIENCE.md`).
**Risks:** Tuning makes the optimal play obvious, collapsing the decision. This is inseparable from chase/energy tuning being finicky. *Update (2026-07-03 human pivot):* the endurance inversion — prey always out-sprint you; you win only by out-lasting — makes the "just sprint" collapse structurally impossible, which strengthens this test. The risk moves to pacing instead (see H14).
**Status:** Open.
**Prototype coverage:** Yes.

### H3 — Players describe the experience as *being* the animal, not controlling it

**Question:** Does the prototype produce presence — the feeling of inhabiting a body — rather than the feeling of steering an avatar?
**Why it matters:** Presence is the #1 emotion in `GAME_DNA.md`. It's the difference between this game and a generic third-person creature game.
**Confidence:** Low. This is subtle, easy to miss, and hard to force with grey boxes.
**Validation:** Listen to *how* players narrate their session. *Redefined for the human pivot (2026-07-03):* embodied first-person language ("I was exhausted") is *default* for human avatars and no longer discriminates. The signal is now **animal-among-animals framing** — vulnerability, non-centrality, body limits ("they're faster than me — I can't win this," "I'm just another animal out here") vs. protagonist framing ("where's my weapon," "what's my objective").
**Success looks like:** Players spontaneously describe themselves as one vulnerable animal in an indifferent world, bound by a body — not as the hero of the scene.
**Risks:** Grey-box fidelity may suppress presence unfairly, giving a false negative. *Added by the pivot:* a human avatar weakens this hypothesis's diagnostic power overall — the deeper "inhabit another species" estrangement is only truly testable at roadmap stage 4 (second, non-human perspective). Interpret prototype presence signals with that caveat. *Added by the camera pivot (2026-07-03):* presence must now also survive an isometric viewpoint — the wider camera is the strongest detachment force yet introduced. Listen specifically for commander framing vs. creature framing (see H18).
**Status:** Open.
**Prototype coverage:** Partial — testable in principle, but presence may be under-served by prototype fidelity. Interpret a weak signal with caution, not as a verdict.

### H4 — Players create memorable stories without any scripted events

**Question:** Will emergent survival moments become stories players want to retell, with zero authored content?
**Why it matters:** Emergence-over-scripting is the moat (`DESIGN_DECISIONS.md` D6). If systems alone can't generate retellable moments at small scale, the content model is in doubt.
**Confidence:** Medium.
**Validation:** After play, ask players to recount their session. Count the specific, personal, unprompted stories.
**Success looks like:** Players tell a story about *their* decision and its consequence that no designer placed there — and two players' stories differ.
**Risks:** At prototype scale there may be too few interacting systems for rich emergence; a weak result here might reflect thinness, not a broken thesis. Note what was missing before concluding.
**Status:** Open.
**Prototype coverage:** Partial — the simplest possible emergence (one predator, one herd) is tested; richer emergence is deferred.

### H5 — A living herd (not a target) makes the world feel alive

**Question:** Does an autonomous, believably-behaving herd make players perceive the world as alive and indifferent, rather than as a set of targets placed for them?
**Why it matters:** The indifferent-living-world foundation (`GAME_DNA.md`). It's also the cheapest place to test whether "believable AI = the product" holds.
**Confidence:** Medium.
**Validation:** Observe whether players watch the herd, treat it as an entity with its own behavior, and react to it as a living thing (patience, reading it) vs. as spawns.
**Success looks like:** Players spend time observing before acting; they describe the herd's behavior as if it had its own agenda.
**Risks:** Herd AI reading as dumb breaks the illusion instantly. This is a pillar, not polish — under-investing here poisons the result.
**Status:** Open.
**Prototype coverage:** Yes.

### H14 — A persistence hunt compressed to minutes is tension, not tedium

**Question:** Real persistence hunting plays out over hours. The prototype must compress "you can't out-sprint them, only out-last them" into a ~2–6 minute pursuit. Does that read as mounting pressure — or as jogging behind a deer?
**Why it matters:** This is the human pivot's biggest new risk (introduced 2026-07-03). The wolf's burst-chase was naturally minutes-scale; the human's attrition-chase is not. If compression fails, we either bend biology deliberately (pillar 5 — flag it) or the human-first bet weakens and we revisit it.
**Confidence:** Low. No evidence either way; slow-burn tension in a minutes-long loop is a real design unknown.
**Validation:** Chase-tuning sessions during build-order step 4, then naive testers. Watch for *mid-chase decisions* — cutting a corner, choosing to push, choosing to give up — versus passive following.
**Success looks like:** Testers describe a long pursuit as tense ("I almost gave up, then it stumbled"); abandoning a chase feels like a *decision with a cost*, not relief from boredom.
**Risks:** Bad tuning produces tedium and we misread it as the concept failing (false negative). Or we over-compress until it is a wolf chase wearing a human mesh — silently un-testing the hypothesis while appearing to pass it.
**Status:** Open.
**Prototype coverage:** Yes — this is now a core prototype question, tested at build-order step 4.

### H15 — An isometric camera over a 3D world makes the ecosystem more readable

**Question:** Does the wider isometric viewpoint make the living system — herd movement and cohesion, pursuit geometry, terrain and elevation, prey fatigue — dramatically more legible than a shoulder camera, so that *reading the simulation* becomes the core player skill?
**Why it matters:** This is the camera pivot's founding claim (`DESIGN_DECISIONS.md` D14, 2026-07-03). The ecosystem is the product; if the isometric view doesn't make it meaningfully easier to read, the pivot loses its main justification.
**Confidence:** Medium. Strong precedent for isometric readability (Project Zomboid's systemic clarity is the direct inspiration), but those games are not embodied single-animal experiences.
**Validation:** Prototype playtests. Watch whether testers actually *use* the wider view — reading herd shape, tracking a chosen animal's fatigue, spotting terrain opportunities — when making decisions.
**Success looks like:** Testers narrate reads unprompted ("that one's tiring — and the rise cuts its corner") and make mid-chase decisions based on what they can see, not on trial and error.
**Risks:** Readability without embodiment — the view informs but detaches (see H18). Or grey-box tells are too crude to read at camera distance, producing a false negative about the perspective rather than the fidelity.
**Status:** Open.
**Prototype coverage:** Yes — this is the camera pivot's core prototype question.

### H16 — A wider perspective strengthens emergent storytelling

**Question:** Does seeing more of the simulation at once produce more — and richer — retellable stories (H4), because players *witness* system collisions instead of only experiencing their own thread of them?
**Why it matters:** Emergence-over-scripting is the moat. If the camera lets players watch stories happen around them (a herd splitting, a straggler isolated, pressure building), the same simulation generates more perceived life per unit of simulation cost (relates to H13).
**Confidence:** Medium.
**Validation:** In post-play story recounting (same protocol as H4), count stories that include events the player *watched* versus only events they *did*.
**Success looks like:** Stories that contain other animals' arcs ("while I was chasing mine, the rest of the herd doubled back behind me…") — narrative that a shoulder camera could not have delivered.
**Risks:** Seeing everything drains mystery and tension; omniscience could weaken the vulnerability that powers presence. The right amount of visibility is an open question (Q18), not a free lunch.
**Status:** Open.
**Prototype coverage:** Partial — one herd on one small map is the simplest possible test; richer multi-system storytelling is deferred.

### H18 — The isometric camera reduces production complexity without weakening immersion

**Question:** Does the camera pivot actually buy the promised production savings — escaping third-person camera feel, close-up animation fidelity, and cinematic asset expectations — while keeping the "I *am* this animal" presence (H3) intact?
**Why it matters:** Half the pivot rationale is production math for a solo dev; the other half is the risk that we quietly pay for those savings with the game's #1 emotion. Both halves must be tracked, or we'll only notice the half that flatters the decision.
**Confidence:** Low on the immersion half; Medium-High on the savings half.
**Validation:** Savings — log concrete avoided work in `BUILD_LOG.md` as it happens (animation polish skipped, camera systems not built, asset quality bars lowered). Immersion — the same presence-listening protocol as H3, now specifically listening for *commander language* ("send him over there," "my unit") versus *creature language* ("I'm exhausted," "I can't win this sprint").
**Success looks like:** The build log shows real avoided work, and testers still self-describe as the animal, not as its operator.
**Risks:** Presence loss is subtle and we rationalize it because the pivot is already made. Or the savings get eaten by new costs the camera creates — readability VFX, fatigue tells that work at distance, terrain that reads from above.
**Status:** Open.
**Prototype coverage:** Partial — the immersion half is testable now; the production-savings half accrues as evidence over the whole build.

---

## Tier 2 — Hypotheses the current prototype canNOT answer (deferred, do not build for these yet)

These matter enormously but require systems, species, or scale the prototype deliberately excludes. Listed so we test them *later, on purpose* — and so no one uses them to justify expanding the prototype now.

### H6 — Prey gameplay can be as engaging as predator gameplay

**Question:** Can inhabiting a prey animal (survival through vigilance and escape) be as compelling as inhabiting a predator (survival through the hunt)?
**Why it matters:** Half the "every species is a different game" thesis. If only predators are fun, the vision narrows drastically.
**Confidence:** Unknown. We have a hunch it can, but no evidence, and predator play is the culturally easier sell.
**Validation:** Requires a playable prey species — roadmap stage 4, not now. Compare engagement of prey vs. predator sessions.
**Success looks like:** Prey sessions produce their own distinct tension and stories ("I drifted too far from the herd…") at comparable engagement.
**Risks:** Prey play collapses into passive waiting; or we over-correct and give prey predator-like verbs, betraying the point.
**Status:** Open.
**Prototype coverage:** No — deferred. Must not pull a second species into the current prototype.

### H7 — Possession architecture actually simplifies adding new playable species

**Question:** Does building the player as a possessed AI agent genuinely make the *second* species cheap (input/camera/tuning) rather than a rewrite?
**Why it matters:** The central technical bet (`DESIGN_DECISIONS.md` D7). The entire "many perspectives" future depends on it being true.
**Confidence:** Medium. Architecturally sound in theory; unproven in practice until we do it twice.
**Validation:** The prototype builds the possession seam (togglable AI/player agent — built first on the wolf) now, but the hypothesis is only *proven* when we make a second, different species playable with low effort. *Early signal (2026-07-03):* the wolf→human pivot is an unplanned live test — if the swap lands as mesh + config + tuning with no core rewrite, confidence here rises before stage 4 even starts.
**Success looks like:** Second playable species reuses the agent foundation with mostly data + input work, no core rewrite.
**Risks:** Hidden coupling surfaces only at the second species; the abstraction leaks and we pay the rewrite we tried to avoid.
**Status:** Open.
**Prototype coverage:** Partial — the prototype builds and validates the *seam* (can we toggle the agent between AI and player?), but not the full claim, which needs species #2.

### H8 — Every species should perceive the world differently — and it's worth the cost

**Question:** Should different species literally sense the world differently (scent-led wolf, vigilance-led prey), and does that difference justify its complexity?
**Why it matters:** It's the mechanism behind "each species is a different game." But per-species perception is expensive and could be over-engineering.
**Confidence:** Low on the "worth the cost" half. High on it being *desirable* in principle; unproven that it's *necessary*.
**Validation:** Deferred. First test whether a single perception model already produces distinct-feeling species; only add per-species perception if sameness is the actual problem.
**Success looks like:** Distinct perception measurably changes how a species is played, and players notice.
**Risks:** Gold-plating: building elaborate per-species senses before knowing they're needed. A textbook `DESIGN_PRINCIPLES.md` #7 trap.
**Status:** Open.
**Prototype coverage:** No — the prototype uses one simple line-of-sight model for all. Deferred by design.

### H9 — How much realism improves gameplay before it starts to frustrate

**Question:** Where is the line past which added biological fidelity subtracts from fun?
**Why it matters:** D2/D3 and `DESIGN_QUESTIONS.md` Q12. This is a curve, not a point, and we need to learn its shape rather than guess it.
**Confidence:** Unknown. This can only be learned empirically, feature by feature.
**Validation:** Ongoing, per-feature comparison of "more realistic" vs. "more legible/fun" variants as systems are added. Not a single test.
**Success looks like:** A working heuristic emerges for where realism helps vs. hurts, logged as rulings in `DESIGN_DECISIONS.md`.
**Risks:** We set the line by taste/mood instead of evidence; or we treat one feature's answer as universal.
**Status:** Open.
**Prototype coverage:** No — the prototype is intentionally low-realism; there's nothing to trade off yet.

### H10 — Emergent gameplay can replace traditional quests/objectives

**Question:** Can a systems-driven world sustain long-term engagement without any authored goal structure?
**Why it matters:** If false, we may need *some* authored scaffolding — which collides with the DNA and must be designed very carefully.
**Confidence:** Low for the long term. H1 tests the 10-minute version; sustaining it over 20 hours is a different, harder claim.
**Validation:** Only answerable with a much larger build and longer sessions. Far future.
**Success looks like:** Players return across many sessions self-motivated, with no objective system.
**Risks:** Short-term engagement (H1) gives false confidence about long-term; the "empty after the novelty" wall appears only at scale.
**Status:** Open.
**Prototype coverage:** No — the prototype tests short-session engagement only. Explicitly do not conflate H1's result with H10.

### H11 — Death should create attachment, not just frustration

**Question:** Can death (and loss) be tuned so it deepens attachment to the life you inhabited, rather than simply annoying the player?
**Why it matters:** `DESIGN_QUESTIONS.md` Q1/Q10. Death is where the DNA's stakes become real and where players most easily quit.
**Confidence:** Low. The knife-edge between meaningful and punishing is narrow and personal.
**Validation:** The prototype's fragile-survivor tuning gives *first* signal on how death *feels*; how death *persists* (permadeath vs. continuity) is a later, separate test.
**Success looks like:** Players re-engage after death wanting to try again, and speak about the lost run as *theirs*.
**Risks:** Death reads as unfair or grindy; players bounce. Or we soften it into meaninglessness to retain them, killing the stakes.
**Status:** Open.
**Prototype coverage:** Partial — the prototype tests the *feel* of dying, not the *structure* of death/persistence.

### H12 — An indifferent world can be made approachable without compromising its identity

**Question:** Can we onboard players into an indifferent, emergent, animal-sensed world without softening the indifference that defines it?
**Why it matters:** The single deepest tension in the project (`DESIGN_DECISIONS.md` D11, `DESIGN_QUESTIONS.md` Q11). Possibly the difference between a cult classic and something no one can get into.
**Confidence:** Unknown. This likely has no single answer — more a set of techniques discovered over time.
**Validation:** Repeated first-contact playtests over the project's life, watching where new players bounce and why, testing legibility techniques that don't hand-hold.
**Success looks like:** New players get past first contact and into the loop at a healthy rate *without* the world being bent toward them.
**Risks:** Every "fix" for approachability nudges the world toward caring about the player, eroding the DNA one comfort at a time. This is the most dangerous hypothesis to test carelessly.
**Status:** Open.
**Prototype coverage:** Partial — the prototype gives early first-contact signal (do naive players get in at all?), but full approachability is a long-horizon question.

### H13 — How much of the ecosystem simulation is actually perceptible to players

**Question:** Of the simulation we build, how much does the player actually notice — and are we paying to simulate things no one perceives?
**Why it matters:** Directly governs where simulation effort is worth spending. Invisible simulation is cost without payoff; the risk is huge for a solo dev.
**Confidence:** Unknown.
**Validation:** As systems are added, test whether players notice/react to them; cut or fake what isn't perceived. Ongoing.
**Success looks like:** A clear read on the minimum simulation depth that produces the "alive world" feeling — so we can stop there.
**Risks:** We over-simulate for our own satisfaction; or we under-simulate and the world feels shallow in ways players sense but can't name.
**Status:** Open.
**Prototype coverage:** Partial — even at prototype scale, note which herd behaviors players actually perceive vs. which we built for ourselves.

### H17 — Species perception can be represented visually from the isometric view

**Question:** Can per-species senses be expressed visually from the wider camera — overlays, vision cones, scent trails, sound indicators, field-of-view shading — so that "how this body senses the world" is readable from above, without collapsing into UI soup?
**Why it matters:** This is the mechanism behind "every species is a different game" under the new camera, and the sibling of H8. In third person you could *inhabit* a sense (screen effects, audio); isometric must *visualize* it. If senses can't be shown from above, the species-perception thesis and the camera decision are in tension.
**Confidence:** Low-Medium. Rich precedent for individual techniques (vision cones, sound rings, scent trails in stealth and sim games); unproven that a whole sensory identity per species reads clearly and stays immersive.
**Validation:** Deferred — the prototype uses plain line-of-sight with no overlays. The first real test rides with the scent/tracking system (roadmap stage 2): can a scent-led perception layer change player behavior without breaking the fiction?
**Success looks like:** Player behavior visibly changes with the represented sense (moving downwind of a scent trail, freezing inside a vision cone) and players describe the sense as *the animal's*, not as a UI feature.
**Risks:** Overlay soup kills diegesis and the indifferent-world feel; or we gold-plate gorgeous sense visualizations nobody uses (the H13 trap). Also relates to Q18/Q19 — visibility and perception representation must be decided together.
**Status:** Testing — *updated 2026-07-03 (twice):* the first build (overlays annotating an omniscient view) was judged the **wrong abstraction** the same day — presentation changed, information didn't, so no decision changed. Redesigned in `PERCEPTION_DESIGN.md` around *information availability*: the screen renders what the body knows. Slice 1 is implemented — vision-only Absent/Tracked gating (cone + range + line of sight, percepts never fabricate), tuned for human vs. big cat; v1 overlays demoted to a debug toggle. The hypothesis under live test is now sharper: *does losing information feel like inhabiting a body, or like being cheated?*
**Prototype coverage:** Partial — slice 1 tests knowledge-gated vision only. The Detected/Remembered rungs (deer's motion sense, wolf's scent-as-memory) and attention postures are designed but unbuilt; per-species senses driving *AI gameplay* remains deferred.

---

## Working the log

- **Every sprint names its hypotheses.** Before building, state which H-numbers the work is meant to move. Work that maps to no hypothesis is scope creep — send it to `DESIGN_QUESTIONS.md` or cut it.
- **Update on evidence, not opinion.** Change a confidence or status only when there's a real signal. Record what the signal was.
- **Rejected is a win.** A Rejected hypothesis with a clear reason is permanent, valuable knowledge. Keep it, dated.
- **New uncertainty gets added immediately.** The moment a build reveals a new unknown, it becomes an H-entry rather than a silent assumption.
- **This document outranks the plan.** If the hypotheses say we've been building toward the wrong bet, the plan changes — not the log.
