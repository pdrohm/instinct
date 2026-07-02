# Design Questions

> A living list of hard questions to resolve *before* production — not now.
> These are deliberately unanswered. The value is in holding them open, framed well, so we answer them with evidence instead of stumbling into defaults.
> Governed by `GAME_DNA.md` and `DESIGN_PRINCIPLES.md`. When a question is answered, record the answer and its reasoning in `DESIGN_DECISIONS.md`, then mark it resolved here with a pointer.

## How to use this document

Do not rush to answer these. Rushing produces a default, and a default chosen under pressure usually serves convenience over the DNA. Each question below includes *why it is hard* and *what pulls in each direction*, so that when the time comes — usually when the prototype or a later slice gives us real signal — we argue it from a good starting frame.

Add questions freely. A question here is cheaper than a wrong decision later.

Priority tags:
- **[CORE]** — touches the DNA; getting it wrong changes what the game *is*.
- **[SYSTEM]** — shapes a major system; important but not identity-level.
- **[LATER]** — real, but safely deferred well past the prototype.

---

## Open questions

### Q1 [CORE] — How punishing should death be?

**Why it's hard:** Death is where "one indifferent life" becomes real — but it is also where players quit. Permadeath maximizes weight and stakes; it also maximizes frustration and can punish the exploration we want to encourage.
**Pulls:** Toward harshness — stakes, meaning, the DNA's indifferent world. Toward softness — retention, onboarding, willingness to take the risks that make stories.
**Do not answer by default.** The prototype's fragile-survivor tuning will give first real signal on how death *feels* before we decide how it *persists*.

### Q2 [CORE] — How central is death vs. continuity of a life?

**Why it's hard:** Is the unit of play "one animal until it dies" or "a life you inhabit across time"? This interacts with Q1 and with the future season/lifespan arc. It changes whether the emotional peak is *dying* or *enduring*.
**Pulls:** Toward discrete lives — clean stakes, roguelike legibility. Toward continuity — attachment, the "your story" arc, deeper loss.

### Q3 [SYSTEM] — How should injury work — or should it exist at all?

**Why it's hard:** Injury deepens consequence (a failed chase that leaves you limping is a great story) but risks a death-spiral where one setback guarantees the next, which feels unfair rather than tense.
**Pulls:** Toward injury — richer consequence, bodily presence. Against — complexity, death-spirals, tuning burden. *Note: not in prototype scope; do not let this pull the prototype bigger.*

### Q4 [SYSTEM] — How much randomness should exist, and where?

**Why it's hard:** Emergence needs variability, but randomness that decides outcomes feels unfair, while fully deterministic worlds feel gameable and dead. The question is not "how much" but "where" — randomness in *world conditions* reads very differently from randomness in *action resolution*.
**Pulls:** Toward randomness — surprise, replayability, life's unpredictability. Against — perceived fairness, player agency, readability.

### Q5 [CORE] — How intelligent should the AI *appear* (vs. actually be)?

**Why it's hard:** Believability is the product, but perceived intelligence and actual intelligence are different problems. Cheap tricks can read smarter than expensive simulation; over-smart animals can feel unfair or uncanny. There is also a floor set by animation/readability (see `DESIGN_DECISIONS.md` D4).
**Pulls:** Toward more — immersion, the "alive" feeling. Toward less/faked — cost, and the risk that too-clever animals stop feeling like *animals* and start feeling like opponents.

### Q6 [CORE] — How do we prevent every species from collapsing into combat gameplay?

**Why it's hard:** Combat is the easiest legible conflict to build and the strongest gravity well in the genre. Every survival mechanic can be lazily expressed as "fight or flee." The DNA forbids the game becoming combat-centric, but nothing structural yet prevents each new species from drifting there.
**Pulls:** Toward combat — it's legible, marketable, and easy. Against — it flattens the "every species a different game" promise into "every species a different fight."
**This one needs an active answer, not just vigilance.** What is the *positive* alternative verb-space that keeps a horse's game about vigilance and a mammoth's about migration?

### Q7 [CORE] — How do we prevent optimization from replacing immersion?

**Why it's hard:** Given any system, players will find the optimal loop and stop *inhabiting* to start *executing*. The moment a player is min-maxing a hunger bar instead of feeling hungry, the DNA is broken. This is arguably the deepest design problem the game has.
**Pulls:** Systems legible enough to engage vs. opaque/organic enough to resist being solved. Related to Q4 (randomness) and Q5 (AI).

### Q8 [SYSTEM] — How much should the game teach, and how?

**Why it's hard:** The DNA says teach through inhabiting, not tutorials. But an indifferent, emergent world with animal senses is genuinely hard to enter cold. There is a real risk that "no tutorial" means "no one gets past the first ten minutes."
**Pulls:** Toward teaching — accessibility, retention. Against — immersion, the purity of discovery. This is Q11's twin from the player's first-contact side.

### Q9 [CORE] — What should the player remember after one hour? After twenty?

**Why it's hard:** Defines the actual payoff curve. If the answer to "after one hour" is "not much," the game has an onboarding-shaped hole. If "after twenty" is "the systems, not a story," the emergence promise failed. We need to know what memory we are aiming for at each horizon.
**Pulls:** N/A — this is a target-setting question, not a tradeoff. It should be answered early because it disciplines everything else.

### Q10 [CORE] — How far do we push loss (offspring, herd, bonds)?

**Why it's hard:** Loss is the most powerful emotion the game can reach and the easiest to make feel cruel or manipulative. Too little and the world has no stakes; too much or too cheap and it feels punitive or exploitative.
**Pulls:** Toward depth — unforgettable moments, real attachment. Against — player wellbeing, fairness, the line between meaningful and gratuitous.

### Q11 [CORE] — How do we make an indifferent world approachable without betraying its indifference?

**Why it's hard:** This is the central tension of the entire project (`DESIGN_DECISIONS.md` D11). The indifferent world is the soul; it is also the most likely reason a new player bounces. Softening it toward the player kills the DNA; not softening it may cap the audience severely.
**Pulls:** Everything. This is *the* question the whole design orbits. It likely does not have a single answer — more a set of techniques (legibility without hand-holding, framing, pacing of first contact) discovered over time.

### Q12 [SYSTEM] — How much scientific realism are we ever willing to sacrifice, and who decides?

**Why it's hard:** D2/D3 establish that gameplay wins and accuracy inspires — but "how much, this specific time" is a judgment call made repeatedly forever. Without a rule of thumb, the line drifts based on mood.
**Pulls:** Toward realism — credibility, the scientific-foundation identity, respect. Toward gameplay — fun, legibility, pace.
**Needs:** not a final answer, but a *heuristic* for making the call consistently and a habit of logging each ruling.

### Q13 [LATER] — Does inhabiting a second species actually feel like a different game?

**Why it's hard:** The entire long-term thesis (`GAME_DNA.md`) rests on this, and it cannot be answered until we have a second playable species (roadmap stage 4). If species feel merely reskinned, the vision needs rethinking. Flagged now so we test it deliberately rather than assume it.
**Pulls:** N/A — an empirical bet to be validated, not a preference to be chosen.

### Q14 [LATER] — When, if ever, does multiplayer justify its cost?

**Why it's hard:** D8 defers it as a consequence, not a goal. But "never decide" is itself a decision. We need a *trigger condition* — what evidence would make netcode worth the scarce time — rather than a vague "later."
**Pulls:** Toward building — social stories, reach, the shared-world dream. Against — cost, and the risk of scaling before the core earns it.

---

## Questions to add as we go

The list is meant to grow. Good candidates for future entries: pacing of scarcity/seasons, how the player perceives their own animal's internal state without breaking immersion, how much persistence the world should have, what "progress" even means in a game with no win condition, and how success is measured commercially without corrupting the DNA.

Whenever a decision feels like it's being made by default rather than on purpose, stop and write it here first.
