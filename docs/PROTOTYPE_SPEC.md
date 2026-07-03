# Prototype Spec — "The First Life"

> Companion to `VISION.md`. This spec defines the single bet worth making first.
> Rule: if it is not required to answer the core question, it is out of scope. No exceptions without a written reason.
> **2026-07-03 pivot:** first playable species changed from wolf to early *Homo sapiens*. Rationale below and in `BUILD_LOG.md`. The systems under test did not change — the body did.
> **2026-07-03 camera pivot:** gameplay perspective changed from third-person to an **isometric camera over the same fully 3D world** (`VISION.md`, `DESIGN_DECISIONS.md` D14). Scope is unchanged — same systems, same small map, same success criteria at heart. The viewpoint changed, not the game.

## What we are actually validating

The vision is *"living as another species inside a scientifically grounded prehistoric ecosystem."* The prototype validates the load-bearing assumption underneath that:

**Is inhabiting an animal inside a living ecosystem inherently engaging through simple survival systems alone — with zero scripted content?**

The camera pivot adds a **secondary perspective question that rides along for free — no new systems**: does the isometric 3D viewpoint make survival decisions, the pursuit, and the stamina economy *easier to read and more interesting* than a shoulder camera would — while still feeling like being one animal, not commanding one? (H15, H18.)

The player's first body is an early human — but an early human played *as an animal*: no tools, no crafting, no technology. Same rules as everything else alive on the steppe. Hungry, slow in a sprint, and bound by a body.

Why the human, and not the wolf we started with:

1. **The stamina economy cannot collapse.** A predator that can out-sprint prey lets tuning quietly erase the decision ("just sprint"). A human can *never* win the sprint — prey are faster in bursts; humans win by endurance, or not at all. The "is this chase worth committing to?" decision is load-bearing *by construction*, which is exactly the hypothesis (H2) the prototype exists to test.
2. **The mechanic was already persistence hunting.** The original spec said pursuit is won "by out-lasting or cutting off a straggler, not by being unconditionally faster." That *is* persistence hunting. The human body makes the fiction honest to the mechanic.
3. **The cheapest believable body.** Humanoids get the entire retargeting/animation/marketplace ecosystem for near-zero cost; believable quadruped locomotion is expensive and rare. "Art is buyable" is far more true for a human — and a janky wolf would have unfairly suppressed the presence test (H3).
4. **The science is exceptional.** The endurance running hypothesis (Bramble & Lieberman 2004; Carrier 1984) and documented persistence hunts are peer-reviewed ground. "Science inspires mechanics" rarely gets a cleaner example.

What the pivot costs — stated honestly: the "inhabit another *species*" estrangement now rests on future species (roadmap stage 4), and the presence signal (H3) must be redefined, since embodied language is default for human avatars. Both are logged in `HYPOTHESES.md`. And compressing an hours-long persistence hunt into minutes of gameplay is unproven — that is the prototype's new make-or-break question (H14).

To be explicit about the framing: we are **not** asking "is hunting fun?" We are asking "is *living as an animal* engaging?" The hunt is just this body's version of the universal loop — sense, decide, commit, resolve, live with the consequence.

## Success criteria (write these down, test against them)

The prototype succeeds if, in a ~10-minute session, a naive tester **unprompted**:

1. Describes a specific survival decision they made and its consequence ("I should have waited / skipped that / given up on that chase sooner").
2. Feels the energy economy (hunger vs. stamina) as tension, not as a chore or a UI bar to babysit.
3. Talks about the experience as being *an animal among animals* — vulnerable, non-central, bound by a body — rather than as a protagonist the world is arranged around.
4. Wants to try again with a different approach after failing.
5. Reads the situation from the wider view — prey fatigue, herd shape, distances, terrain — and uses that reading to decide (cut the corner, keep pushing, abandon the chase).

It fails if testers treat the human as a generic game avatar, ignore the energy economy, feel the herd is a dumb target rather than a reactive, alive part of the world, or if the long pursuit reads as tedium rather than mounting pressure. It also fails if the isometric view makes them feel like they are *commanding a unit* rather than *being the animal* — that is the camera failing its job, and it is a result we want to catch early. Failure is a valid, valuable outcome — it saves a year.

## Player fantasy for the slice

You are an early human on cold, open ground — and you are *only* an animal here. No spear, no fire, no plan beyond your body. You are hungry. The herd is aware of you. They are faster than you and always will be; what you have is endurance, patience, and the ability to read the land. Every choice about *whether, when, and how* to commit to a pursuit is yours, and the world answers honestly. The story that comes out of ten minutes is *your* story of one animal surviving, not a level you completed.

> Emotional target is an **open question** (see `VISION.md`): tense-hunter mastery vs. fragile-survivor. Default tuning for the first build leans *fragile survivor* — failure common and cheap to retry — because it stress-tests the energy economy hardest and best reflects "one vulnerable life inside an indifferent world."

## In scope — the minimum that tests the question

Implementation is **identical** to the lean slice already built for the wolf — same agent architecture, same components, new body and tuning. The reframing changes what we *measure and value*, not what we *build*.

### World
- One small map, one biome: open Ice Age steppe. A few line-of-sight breakers (rocks, a rise, sparse cover) and readable elevation. No streaming, no weather, no time of day.
- Hard boundaries. No navigation beyond the play space.

### The player agent (early Homo sapiens)
- **Isometric camera over the 3D scene** — angle, distance, and rotation policy are open tuning questions (`DESIGN_QUESTIONS.md` Q15–Q16); direct WASD-style embodied control is the default hypothesis, with click-to-move an open question (Q17). Move + sprint.
- **Stamina**: drains while sprinting, regenerates while walking/resting. Sprinting closes distance briefly but can never outrun prey outright — stamina *is* the action budget, and endurance is the only winning strategy.
- **Hunger**: a slow clock. Rises over time; forces action. Feeding resets it. Starvation ends the run.
- One action verb: **pursue** (sprint/press to keep prey moving and deny them recovery). Contact with an exhausted prey = feed. That is the entire interaction model — no attack timing, no throwing, no health bars. Keep it crude on purpose; the point is the *decision*, not the mechanic.

### The world around you (one herd species — reindeer or steppe horse)
- 6–12 animals moving as a **herd** (cohesion + separation + alignment; simple boids is fine). The herd is not "the target" — it is a living part of the world that happens to also be food.
- **Awareness**: each prey has a detection radius / vision. When the human is detected within a threshold, the herd **flees**; distance and speed modulate panic.
- **Stamina on prey too — inverted against you**: prey sprint *faster* than you but burn stamina harder and recover slowly while pressed. You win by keeping a chosen animal moving, cutting corners across terrain, and never letting it rest — persistence hunting in miniature. This asymmetry is the crux of whether the world feels fair and alive.
- Prey that escape far enough (or that you stop pressing) calm down, recover, and return to grazing.

### Feedback (minimal, diegetic where possible)
- Readable stamina indicator for the player.
- A readable *prey* fatigue tell (gait/posture degradation — animation states, not UI), because the entire commit/abandon decision hinges on reading it. **It must read at isometric camera distance** — silhouette and gait, not close-up detail.
- A hunger state the player can *feel* (screen/audio cue as it gets dangerous — not a numeric obsession).
- Clear feed and clear death moments. Nothing else.

## Environment & character: bought, not built

Goal: **immersion, not realism** — the environment should already whisper *Ice Age steppe* without a production art pass. Rules:

- **All art is acquired**: Fab / Quixel Megascans (terrain, rocks, ground surfaces), marketplace foliage packs (sparse steppe grasses/shrubs — not forests), marketplace terrain assets.
- **The player character is a marketplace/Fab prehistoric or "primitive human" character**, retargeted to standard humanoid locomotion animations. Anatomically believable is enough. **Zero custom modeling, zero custom animation.**
- The dressing must serve gameplay first: open sightlines, natural LOS blockers, interesting elevation, readable silhouettes at distance. Everything is judged **from the isometric camera's height and angle** — ground readability and silhouettes trump close-up detail. If an asset hurts readability or frame rate, cut it — sparse and legible beats dense and pretty.
- **Timebox the entire environment/character pass.** It is set dressing for a hypothesis test, not a deliverable. When the timebox ends, it ships as-is.

## Explicitly OUT of scope (do not build these yet)

- **No tools, no weapons, no throwing.** The human is an animal in this prototype. This is the wall that keeps us out of "human survival game" territory.
- **No crafting, no combat system, no shelter building, no fire, no inventory, no progression, no tech.**
- No second playable species. (The *code* supports possession — see below — but we expose only the human.)
- No other humans, no band/tribe, no rival predators.
- No reproduction, aging, seasons, or lifespan arc.
- No scent/tracking system (Stage 2 — the prototype uses direct line-of-sight only).
- No day/night, no cold/warmth pressure.
- No multiplayer, no netcode.
- No Mass Entity. No thousands of animals.
- No production art pass beyond the acquired-assets rules above. No audio design pass.
- No save system, no menus beyond restart.

A playable human makes several of these *more* tempting, not less — players will ask for a spear within minutes. That pull is the scope-creep risk from `VISION.md` showing up on schedule. Write down why before adding anything.

## Technical approach (deliberately un-fancy)

- **Engine:** Unreal Engine 5.
- **Camera:** isometric / top-down over the fully 3D scene. Standard UE5 pipeline throughout — 3D terrain, depth buffer, lighting, navmesh, animation. **No 2D tile engine, no custom engine.** The perspective is a camera rig, not an architecture.
- **Animals as ordinary Actors**, not Mass entities. A dozen animals does not need ECS.
- **AI via State Tree (or Behavior Trees)** — graze / alert / flee / return for prey; idle / wander for the human's resident brain (unused while the player drives, but present so the human is a genuine agent like everything else).
- **Possession from day one.** The human is an AI agent whose decisions are handed to player input — identical seam to the wolf build. This is non-negotiable; it *is* the architectural bet from `VISION.md`. Note: the wolf→human swap is itself the first evidence for that bet (H7) — if the swap is genuinely mesh + data + tuning, the architecture is doing its job.
- **Blueprints-first for feel iteration;** C++ where already established. Do not pre-optimize.
- **Data-driven agent config.** Needs, senses, and speeds live in data assets (`UAnimalConfig`), not hardcoded — the human is a config, exactly as the wolf was.
- **Art is buyable.** See environment/character rules above. Zero solo time on modeling or animation.

## Smallest testable slices (build order within the prototype)

Each step should be independently playable and answer something. Do not proceed until the current one feels right.

1. **Human + stamina, seen isometrically.** Move and sprint through the human's body on the steppe, from the isometric camera. Does inhabiting it feel good *from the wider view*? Does the stamina budget create a rhythm? This is the first read on whether embodiment survives the camera. *(Carries over from the wolf slice — swap body, retune config, swap the camera rig.)*
2. **Add a living herd.** Boids-based grazing herd that ignores you. Does the world read as alive on its own?
3. **Add awareness + flee.** Herd detects and flees. Does approaching become a *decision* — a read of the situation?
4. **Add prey stamina + the feed — the persistence duel.** Prey outsprint you but tire when pressed; pursuits end in a feed or an abandoned chase. *This is the core loop — and the compression test (H14). Make-or-break.*
5. **Add hunger.** The clock that forces the loop to repeat under pressure. Does the whole thing now feel like *surviving as an animal*, not completing a task?

If step 4 is not engaging, stop. Steps past it will not save it.

## Risks specific to this prototype

- **The RTS drift (new with the camera pivot).** An isometric view invites detachment — the player as commander of a unit rather than an animal in a body. Controls and feedback must keep pulling toward embodiment: direct control, body-anchored feedback, no unit-selection idioms. If testers start talking like generals, the camera is failing its job (H15/H18, Q20).
- **Readability at camera distance.** Prey fatigue tells, stamina states, herd behavior, and terrain must all read from the isometric camera's height and angle, or the core commit/abandon decision goes blind. Grey-box tells that are too crude to read at distance would produce a false negative on the whole prototype.
- **Pacing is the new #1 risk (H14).** Real persistence hunts take hours; ours must compress to minutes without reading as "jogging behind a deer." Chase/energy tuning is not a detail — it *is* the fun. Budget real iteration time on speeds, stamina curves, and recovery-denial rules.
- **Human-shaped scope gravity.** A human avatar invites tools/fire/crafting expectations from testers and from us. The out-of-scope wall above exists for this.
- **The world reading as dumb.** Even at prototype fidelity, a herd that flees stupidly breaks the "living world" promise. Spend here; it is a pillar, not polish.
- **Presence signal is muddier with a human (H3).** Embodied language is default for human avatars. Judge presence by *animal-among-animals* framing — vulnerability, non-centrality, body limits — not by first-person phrasing alone.
- **Art acquisition as procrastination.** Browsing Fab is not development. Timebox it; ugly and alive beats pretty and unproven.

## Definition of done for the prototype

Not "it works." Done means: **we have run it past at least a handful of naive testers and gotten a clear read on the core question** — a confident yes, a confident no, or a specific "yes if we fix X." Any of those three is a win, because all three tell us whether the ecosystem-as-product thesis is worth building on.
