# Prototype Spec — "The First Life"

> Companion to `VISION.md`. This spec defines the single bet worth making first.
> Rule: if it is not required to answer the core question, it is out of scope. No exceptions without a written reason.

## What we are actually validating

The vision is *"living as another species inside a scientifically grounded prehistoric ecosystem."* The prototype validates the load-bearing assumption underneath that:

**Is inhabiting an animal inside a living ecosystem inherently engaging through simple survival systems alone — with zero scripted content?**

The wolf's hunt is only *one expression* of that. We use the wolf because it is the cheapest, most legible way to put a player inside the simulation and find out whether *being an animal in this world* holds attention on its own. If the answer is yes for the wolf, we have strong evidence the same simulation will support a horse, a mammoth, or a human as different lenses later. If the answer is no, no amount of extra species will save it — and we have learned that for the price of one animal.

To be explicit about the framing shift: we are **not** asking "is hunting fun?" We are asking "is *living as an animal* engaging?" The hunt is just the wolf's version of the universal loop — sense, decide, commit, resolve, live with the consequence.

## Success criteria (write these down, test against them)

The prototype succeeds if, in a ~10-minute session, a naive tester **unprompted**:

1. Describes a specific survival decision they made and its consequence ("I should have waited / skipped that / not drifted so far").
2. Feels the energy economy (hunger vs. stamina) as tension, not as a chore or a UI bar to babysit.
3. Talks about the experience in terms of *being the animal* — its instincts, its limits — rather than "moving a unit around."
4. Wants to try again with a different approach after failing.

It fails if testers treat the wolf as a generic controllable avatar, ignore the energy economy, or feel the herd is a dumb target rather than a reactive, alive part of the world. Failure is a valid, valuable outcome — it saves a year.

## Player fantasy for the slice

You are inhabiting a wolf — not steering it. Your senses, your body, and your instincts are the wolf's, and they define what you can and cannot do. You are hungry. The herd is aware of you. Your stamina is finite. Every choice about *whether, when, and how* to close the distance is yours, and the world answers honestly. The story that comes out of ten minutes is *your* story of one animal surviving, not a level you completed.

> Emotional target is an **open question** (see `VISION.md`): tense-predator mastery vs. fragile-survivor. Default tuning for the first build leans *fragile survivor* — failure common and cheap to retry — because it stress-tests the energy economy hardest and best reflects "one vulnerable life inside an indifferent world."

## In scope — the minimum that tests the question

Implementation is **identical** to a lean wolf-survival slice. The reframing above changes what we *measure and value*, not what we *build*.

### World
- One small grey-box map, one biome. Open terrain with a few line-of-sight breakers (rocks, a rise, sparse cover). No streaming, no weather, no time of day.
- Hard boundaries. No navigation beyond the play space.

### The player agent (wolf)
- Third-person camera, move + sprint.
- **Stamina**: drains while sprinting, regenerates while walking/resting. Sprinting is the only way to close on prey, so stamina *is* the action budget.
- **Hunger**: a slow clock. Rises over time; forces action. Feeding resets it. Starvation ends the run.
- One action verb: **pursue** (sprint to close distance). Contact with an exhausted/caught prey = feed. That is the entire interaction model for now — no attack timing, no health bars on prey. Keep it crude on purpose; the point is the *decision*, not the mechanic.

### The world around you (one herd species — reindeer or steppe horse)
- 6–12 animals moving as a **herd** (cohesion + separation + alignment; simple boids is fine). The herd is not "the target" — it is a living part of the world that happens to also be food.
- **Awareness**: each prey has a detection radius / vision. When the wolf is detected within a threshold, the herd **flees**; distance and the wolf's speed modulate panic.
- **Stamina on prey too**: prey also tire. Closing distance is a stamina duel — you win by out-lasting or cutting off a straggler, not by being unconditionally faster. This is the crux of whether the world feels fair and alive.
- Prey that escape far enough calm down and return to grazing.

### Feedback (minimal, diegetic where possible)
- Readable stamina indicator for the player.
- A hunger state the player can *feel* (screen/audio cue as it gets dangerous — not a numeric obsession).
- Clear feed and clear death moments. Nothing else.

## Explicitly OUT of scope (do not build these yet)

- No second playable species. No inhabiting UI beyond the wolf. (The *code* supports possession — see below — but we expose only the wolf.)
- No pack, no other wolves, no rival predators.
- No reproduction, aging, seasons, or lifespan arc.
- No scent/tracking system (that is Stage 2 — the prototype uses direct line-of-sight only).
- No day/night, no cold/warmth pressure.
- No crafting, inventory, tech, or progression.
- No multiplayer, no netcode.
- No Mass Entity. No thousands of animals.
- No final art, no audio design pass, no polished animation. Grey boxes and placeholder/asset-pack models only.
- No save system, no menus beyond restart.

If any of these feels tempting mid-build, that is the scope-creep risk from `VISION.md` showing up on schedule. Write down why before adding it.

## Technical approach (deliberately un-fancy)

- **Engine:** Unreal Engine 5.
- **Animals as ordinary Actors**, not Mass entities. A dozen animals does not need ECS.
- **AI via State Tree (or Behavior Trees)** — graze / alert / flee / return for prey; idle / seek / pursue / feed for the wolf brain (unused while the human drives, but built now so the wolf is a genuine agent from day one).
- **Possession from day one, even in the prototype.** The wolf is an AI agent whose decisions are handed to player input. This is non-negotiable — it *is* the architectural bet from `VISION.md`, the thing that makes "inhabit any species" a future feature rather than a rewrite. Building it now costs almost nothing: prove you can toggle the wolf between AI and player control. We build the architecture for many perspectives; we expose exactly one.
- **Blueprints-first.** Iterate on feel in Blueprint; drop to C++ only where profiling demands it. Do not pre-optimize.
- **Data-driven agent config.** Needs, senses, and speeds live in data assets, not hardcoded, so a second species later is authoring data, not writing a class. This is how "every species is a different game on the same simulation" stays cheap.
- **Art is buyable.** Use Fab/Quixel and animal asset packs. Zero solo time spent modeling.

## Smallest testable slices (build order within the prototype)

Each step should be independently playable and answer something. Do not proceed until the current one feels right.

1. **Wolf + stamina.** Move and sprint on an empty map, through the wolf's body. Does inhabiting it feel good? Does the stamina budget create a rhythm?
2. **Add a living herd.** Boids-based grazing herd that ignores the wolf. Does the world read as alive on its own?
3. **Add awareness + flee.** Herd detects and flees the wolf. Does approaching become a *decision* — a read of the situation?
4. **Add prey stamina + the feed.** Pursuits become stamina duels ending in a feed or an escape. *This is the core loop — the make-or-break moment.*
5. **Add hunger.** The clock that forces the loop to repeat under pressure. Does the whole thing now feel like *surviving as an animal*, not completing a task?

If step 4 is not engaging, stop. Steps past it will not save it.

## Risks specific to this prototype

- **Chase/energy tuning is everything and it is finicky.** Too fast and there is no decision; too slow and it is frustrating. Budget real iteration time on speeds and stamina curves — this *is* the fun, not a detail.
- **The world reading as dumb.** Even at prototype fidelity, a herd that flees stupidly breaks the "living world" promise. Spend here; it is a pillar, not polish.
- **Solo-dev perfectionism on art.** The trap is polishing grey boxes. Resist. Ugly and alive beats pretty and unproven.
- **Measuring the wrong thing.** The temptation is to judge this as a hunting minigame. Judge it against the real question: did the tester feel like they were *living as an animal in a world*, not playing a chase?

## Definition of done for the prototype

Not "it works." Done means: **we have run it past at least a handful of naive testers and gotten a clear read on the core question** — a confident yes, a confident no, or a specific "yes if we fix X." Any of those three is a win, because all three tell us whether the ecosystem-as-product thesis is worth building on.
