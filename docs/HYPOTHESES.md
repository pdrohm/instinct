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
**Risks:** Tuning makes the optimal play obvious, collapsing the decision. This is inseparable from chase/energy tuning being finicky.
**Status:** Open.
**Prototype coverage:** Yes.

### H3 — Players describe the experience as *being* the animal, not controlling it

**Question:** Does the prototype produce presence — the feeling of inhabiting a body — rather than the feeling of steering an avatar?
**Why it matters:** Presence is the #1 emotion in `GAME_DNA.md`. It's the difference between this game and a generic third-person creature game.
**Confidence:** Low. This is subtle, easy to miss, and hard to force with grey boxes.
**Validation:** Listen to *how* players narrate their session — the language of embodiment ("I was exhausted," "I was hungry") vs. the language of operation ("I moved it," "the unit").
**Success looks like:** Spontaneous first-person, embodied language about the animal's state.
**Risks:** Grey-box fidelity may suppress presence unfairly, giving a false negative. We may need minimal readability polish before this is testable — but must not let that balloon the prototype.
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
**Validation:** The prototype builds the possession seam (togglable AI/player wolf) now, but the hypothesis is only *proven* when we make a second, different species playable with low effort.
**Success looks like:** Second playable species reuses the agent foundation with mostly data + input work, no core rewrite.
**Risks:** Hidden coupling surfaces only at the second species; the abstraction leaks and we pay the rewrite we tried to avoid.
**Status:** Open.
**Prototype coverage:** Partial — the prototype builds and validates the *seam* (can we toggle the wolf between AI and player?), but not the full claim, which needs species #2.

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

---

## Working the log

- **Every sprint names its hypotheses.** Before building, state which H-numbers the work is meant to move. Work that maps to no hypothesis is scope creep — send it to `DESIGN_QUESTIONS.md` or cut it.
- **Update on evidence, not opinion.** Change a confidence or status only when there's a real signal. Record what the signal was.
- **Rejected is a win.** A Rejected hypothesis with a clear reason is permanent, valuable knowledge. Keep it, dated.
- **New uncertainty gets added immediately.** The moment a build reveals a new unknown, it becomes an H-entry rather than a silent assumption.
- **This document outranks the plan.** If the hypotheses say we've been building toward the wrong bet, the plan changes — not the log.
