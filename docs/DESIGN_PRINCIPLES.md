# Design Principles

> The checklist every proposed mechanic must pass. Derived from `GAME_DNA.md`; enforced against `DESIGN_DECISIONS.md`.
> This is a filter, not a wishlist. Its job is to make saying **"no"** easy and consistent.
> A feature does not need to pass every principle. But a feature that fails *several* probably should not exist.

## How to use this document

For any proposed feature, walk the questions below and answer honestly. Write the answers down next to the proposal. Then apply the verdict rule at the bottom.

The goal is not to score features — it is to force the conversation that stops bad features early and cheaply. A proposal that survives this document with clear answers is worth prototyping. A proposal whose defender has to reach or hand-wave is telling you something.

## The principles

Each is phrased as a question, with *why it exists* and *what a failing answer looks like*.

### 1. Does this create meaningful player stories?

**Why:** Emergent stories are the product (`GAME_DNA.md`). A feature that produces no stories adds cost without advancing the core.
**Failing looks like:** "It's just a nice-to-have." "Players expect it." A feature no one would ever recount afterward.

### 2. Does it increase immersion / the feeling of *being* the animal?

**Why:** Presence is the #1 emotion. Anything that reminds the player they are operating software is working against the DNA.
**Failing looks like:** A menu, a meta-layer, a UI convenience, or an ability that only a *player* (never an animal) would use.

### 3. Does it create interesting decisions instead of busywork?

**Why:** Every action must carry a real tradeoff. A survival system with no decision behind it is a chore bar.
**Failing looks like:** "The player just has to remember to do X every few minutes." Optimal play is obvious and repetitive.

### 4. Can the player discover it naturally?

**Why:** The game teaches through inhabiting, not tutorials. A mechanic that requires explanation to exist is friction.
**Failing looks like:** It only works if a tutorial or tooltip tells the player it's there.

### 5. Does it benefit the ecosystem, or only one species?

**Why:** The ecosystem is the product; species are perspectives. Systems that deepen the shared world scale across every future lens. Species-specific features must justify their narrower payoff.
**Failing looks like:** A one-off mechanic that only ever matters for a single animal and teaches the simulation nothing. *(Note: species-specific is not automatically disqualifying — distinctiveness between species is a goal. But it raises the bar.)*

### 6. Does it strengthen the sense that the world is alive and indifferent?

**Why:** The indifferent living world is the emotional foundation. Features should reinforce it, never soften it into a player-serving theme park.
**Failing looks like:** The world bending toward the player — rubber-banding, difficulty that quietly protects the player, events that only fire for an audience.

### 7. Is there a simpler solution?

**Why:** Emergence needs small systems interacting, not big bespoke features. Complexity is the enemy of both emergence and a solo dev's survival.
**Failing looks like:** A large new system where a small tweak to an existing one would deliver 80% of the value.

### 8. Would removing this feature make the game worse?

**Why:** The cleanest test of necessity. If the game is not clearly poorer without it, it is scope.
**Failing looks like:** "It wouldn't really hurt to cut it, but..." — that sentence is a cut.

### 9. Is the implementation and maintenance cost justified?

**Why:** Every feature is paid for twice: once to build, forever to maintain. For a solo dev on a long haul, this is existential.
**Failing looks like:** High build cost or ongoing tuning burden for a marginal or narrow payoff.

### 10. Does it respect biology as inspiration without becoming a realism tax?

**Why:** Science inspires mechanics; it does not command them (`DESIGN_DECISIONS.md`). A feature added *only* because "real animals do this" — with no gameplay payoff — is realism for its own sake.
**Failing looks like:** "It's more realistic" as the primary justification, with no story, decision, or immersion benefit attached.

## The verdict rule

- **Passes clearly on the core principles (1, 2, 3, 6):** worth prototyping. These four are load-bearing; a feature that fails any of them is suspect no matter how it scores elsewhere.
- **Fails several principles:** it probably should not exist. Cut it, or shrink it until it passes.
- **Passes on paper but you had to reach:** treat the reach as a failure. Honesty here is the whole point.
- **Genuinely unsure:** it becomes a `DESIGN_QUESTIONS.md` entry, not a build task. When in doubt, defer, do not add.

## Standing biases (tie-breakers)

When principles conflict or the call is close, lean these ways by default:

- **Cut over add.** The default answer to a new feature is no.
- **Small over big.** Prefer the smallest system that could produce the desired emergence.
- **Depth over breadth.** One system with many consequences beats many systems with one each.
- **Immersion over convenience.** When a comfort feature threatens presence, presence wins.
- **Emergence over authorship.** Prefer systems that generate stories over content that tells them.
- **Defer over guess.** An unresolved hard question goes to the living questions doc, not into the build.

## Acknowledged tensions (these principles can conflict)

This document is not internally frictionless, and pretending otherwise would make it useless. The known conflicts:

- **"Natural discovery" (4) vs. "interesting decisions" (3).** Deep decision systems can be hard to discover without teaching. We will sometimes have to choose; when we do, note which we sacrificed and why.
- **"Benefits the ecosystem" (5) vs. "every species is a different game" (`GAME_DNA.md`).** Shared systems scale; but if *everything* is shared, species stop feeling distinct. The resolution is deliberate: shared *foundations*, species-specific *expression*. Watch that this line does not blur.
- **"Simpler solution" (7) vs. "believable AI" (`GAME_DNA.md`).** Believability sometimes genuinely requires complexity. Simplicity is a default, not a hard cap — believable-but-complex can beat simple-but-dumb. Justify the complexity when you spend it.
- **"World is indifferent" (6) vs. player onboarding and retention.** An indifferent world can feel aimless or punishing to a new player. This is a real, unresolved tension — see `DESIGN_QUESTIONS.md`. Do not resolve it by quietly making the world care.

Document the conflict when you hit one. Hiding it just moves the argument downstream to a more expensive moment.
