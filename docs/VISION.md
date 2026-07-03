# Game Vision

> Working title: **untitled** — naming is deferred until the prototype tells us what the game actually feels like.
> Status: Concept. Nothing validated yet. This doc is a north star, not a contract.

## One-line pitch

Inhabit another species inside a scientifically grounded prehistoric ecosystem — and let your instincts, senses, and survival decisions write a story no one scripted.

## The fantasy

You are not controlling a character. You are *inhabiting* another form of life. You see the world through its senses, move with its body, and are bound by its instincts, strengths, and weaknesses. A wolf reads the wind and its packmates. A horse lives on awareness and the herd's collective nerve. A mammoth trusts its size and the road its family has always walked. An early human has neither speed nor claws — only sweat, patience, and the ability to keep going long after everything else has to stop. You do not play the animal — for a while, you *are* one.

The world does not revolve around you. Herds migrate, predators compete, seasons turn, and you are one life inside a living system that would carry on without you. Your story is whatever survival makes it.

The feeling we are chasing is the moment a player says, unprompted: *"I got greedy, blew my stamina on a chase I should have skipped, and had to limp off hungry while a rival moved in."* No script wrote that. The simulation did. That sentence should be tellable by every species — the herbivore version is *"I drifted too far from the herd to reach better grazing, and that was the moment I got caught."*

## The real product: the ecosystem

**The ecosystem simulation is the product. Species are perspectives into it.**

We are not building a wolf game, then a horse game, then a mammoth game. We are building one living world — animals with real needs, senses, and instincts, interacting whether or not a human is watching — and then offering different *ways in*. Each playable species is a different lens on the same simulation.

Done right, this means **every future species should feel like an entirely different game**, even though it runs on the same world:

- The wolf is a game about smell, pack, and the economics of the chase.
- The horse is a game about vigilance, the safety of the herd, and knowing when to run.
- The mammoth is a game about migration, memory, and protecting your family.
- The human is a game about endurance and reading the land — and, much later, about planning, cooperation, and tools.

Same world. Same rules of hunger, energy, cold, fear, and death. Radically different experience — because the *body and instincts you inhabit* change what that world demands of you. That is the long-term identity of the project, and it is why the ecosystem, not any one animal, is the thing we invest in.

## Why it can be special

- **Emergence over scripting.** Memorable moments come from AI animals with real needs and senses colliding, not from designer-authored events. That simulation is the moat.
- **You are one life among many, not the center.** This reframes every genre convention and is where the emotional weight lives.
- **One world, many games.** The ecosystem is built once; each species is a new perspective on it, not a new game from scratch.
- **A prehistoric setting with real teeth.** Iconic Ice Age fauna, genuine survival pressure (cold, scarcity, apex competitors), and a scientific foundation that inspires mechanics rather than decorating them.

## Presentation: a fully 3D world, read through an isometric eye

This is a **3D ecosystem simulation presented primarily through an isometric / top-down camera** — not a third-person cinematic survival game. The world stays fully 3D: real terrain and elevation, 3D animals and characters, modern lighting, navmesh, and animation systems in Unreal Engine 5. Only the *viewpoint* changes, and it changes on purpose.

**The camera is a design tool for reading the ecosystem, not a limitation.** From a wider tactical vantage the player can understand what a shoulder camera hides: herd movement and cohesion, predator pressure building at the edges, wind and scent, terrain and elevation as strategy, and the geometry of a pursuit — gaps closing, corners worth cutting, a straggler drifting from safety. The ecosystem is the product; the camera exists to make it legible.

The inspiration is the readability and systemic gameplay of Project Zomboid — **not** its 2D tile-based implementation. We keep the 3D engine and everything it buys us: the asset pipeline, animation quality, depth and lighting, performance scalability, and a real shipping path. No custom engine, no tile engine (`DESIGN_DECISIONS.md` D14).

**And a hard principle: the player is not an RTS commander.** The player is one embodied animal inside the ecosystem. The isometric camera makes the ecosystem readable, but controls, feedback, and decision-making must always make the player feel like they are inhabiting one creature — one body, one set of senses, one life — never managing a squad or commanding a map.

## Design philosophy: every species experiences the same world differently

One world, one set of survival rules, but each species meets that world through a different body and a different set of instincts. The design goal is that the *same terrain, the same herd, the same cold night* should pose a completely different problem depending on who you are inhabiting.

Illustrative examples (directions, **not** a design commitment — we do not design these now):

- **Wolves** rely on smell, pack coordination, and the energy math of the chase.
- **Herbivores (small/fast)** rely on awareness, distance, and the collective nerve of the herd.
- **Large herbivores** rely on size, migration routes, and protecting the young.
- **Predators generally** rely on timing, ambush, and energy efficiency over raw speed.
- **Humans** rely first on the body's one superpower — endurance — and only later on planning, cooperation, and tools.

These are perspectives to *discover through play*, one at a time, long after the core simulation is proven. Listing them here is about direction and identity — not a to-do list.

## The single architectural bet

**The player is a possession layer on top of an ordinary AI-driven animal.**

Every animal — prey, predator, yours — is the same kind of agent: needs (hunger, stamina, warmth, fear), senses (sight, scent, sound), and a decision brain. The AI drives it by default. Being the player just hands one agent's decisions to a human.

This one decision is what makes the whole vision reachable by a solo dev:

- **"Inhabit any species" becomes a feature, not a foundation.** Any competent AI animal can be made playable later — a camera/input/instinct-tuning problem, not a new game.
- **The ecosystem is built once and is the real product.** Emergent stories happen with or without a human watching.
- **It stays open to more players without redesign.** Because the player is just a possessed agent, nothing about the architecture assumes a single human. That is a property we get for free — not a milestone we chase (see roadmap).

## Core loop (any animal, one stretch of survival)

**Sense** (through this species' strengths — sight, scent, sound, herd awareness) → **Decide** (worth the energy? am I exposed? where is safety?) → **Commit** (stalk, flee, migrate, rest, feed) → **Resolve** (fed and safe, or hungry and exposed) → **Consequence** (the outcome changes what you can afford next).

The hunt is only *one expression* of this loop — the predator's. A prey animal runs the same loop toward grazing and escape instead of toward a kill. The tension engine underneath is a simple **energy economy**: acting costs stamina and calories; a mistake costs more than caution. That math is what turns any instinct — chase *or* flee — into a real decision.

## Design pillars

1. **The ecosystem is the product; species are perspectives.** We invest in the world, then open windows into it.
2. **Every decision is a survival tradeoff.** No filler actions. If a choice has no cost, cut it.
3. **The world is indifferent and alive.** Systems run without the player. Stories emerge from their collisions.
4. **Believable animals, not scripted actors.** AI quality is the product. If animals feel dumb, the vision collapses.
5. **Science inspires, never limits.** When gameplay bends biology, we do it on purpose and say why.
6. **Prove fun small before scaling anything.** Validate the smallest meaningful slice before adding species, systems, or players.
7. **One embodied animal, seen from a readable vantage.** The isometric camera serves ecosystem readability; embodiment is non-negotiable. If a choice makes the player feel like a commander instead of a creature, it is wrong.

## What this is NOT (scope guardrails)

- Not a hunting game. Hunting is one species' expression of a universal survival loop.
- Not a human-survival-crafting game. No base building, tech trees, or inventory-as-gameplay. A playable human does not change this: the prototype human has no tools, no fire, no crafting — a human body under animal rules. Tools/cooperation are the human lens's *future* identity, earned like any other system.
- Not "many species" soon. One playable animal first; every other perspective is earned, post-validation.
- Not a persistent MMO. Multiplayer is a property of the architecture, not a destination on the roadmap.
- Not photoreal AAA. Art is bought or stylized. Solo time goes into simulation and AI — the part nobody can buy.
- Not a third-person cinematic game, and not an RTS or god game. A fully 3D world read from an isometric camera — one embodied life, not a command view over many.

## Staged roadmap

The roadmap validates *gameplay*, in order, before it validates *scale*. Each stage is gated on the previous one being fun. We do not skip ahead.

1. **Prototype** — inhabit one early *Homo sapiens*, played strictly as an animal (no tools, no crafting), in a living herd-based ecosystem; validate that simple survival systems (hunger, stamina, awareness) are inherently engaging. *This is the core bet.* (First playable pivoted from wolf to human — rationale in `PROTOTYPE_SPEC.md`.)
2. **The ecosystem breathes** — scent/tracking, day-night + Ice Age cold, a rival predator that competes with and threatens you. The world starts generating stories on its own.
3. **The life, not the moment** — reproduction, social bonds, a season/lifespan arc so "your story" has a beginning and end.
4. **A second perspective** — make one existing AI animal (e.g. the prey species) playable, and confirm the payoff thesis: *inhabiting a different body feels like a different game.* This is where the real vision proves itself.
5. **More perspectives, as the world earns them** — additional species, each a new lens, added only once the previous one is genuinely fun.

**On multiplayer:** it is deliberately *not* a roadmap milestone. Because the player is just a possessed agent, more than one human in the same simulation is something the architecture already permits — a consequence of the design, not a goal we build toward. We will only spend real effort on netcode if and when validated gameplay makes it clearly worth it. Until then, it stays a free option we protect, not a feature we chase.

**On scale:** thousands-of-animals technology (e.g. Mass Entity) is a later optimization, adopted only if herd scale becomes a measured bottleneck — never pre-emptively.

## Biggest risks

1. **Scope creep via "more species / more systems."** The #1 project-killer. Mitigated by the possession architecture deferring every extra perspective to a post-validation feature.
2. **Animal AI is the entire product and is genuinely hard.** Built first, not polished last.
3. **Solo dev vs. AAA-scale ambition.** Continuous right-sizing is a permanent part of the process.
4. **Science vs. fun.** Resolved by pillar 5; every deliberate liberty gets flagged.

## Success definition for the concept

We consider the *vision* validated — not the game finished — when a naive tester, inhabiting the grey-box early human for ten minutes, produces an unscripted survival story they care about, driven purely by simple systems rather than scripted content. That result is the green light for everything above, because it proves the one thing the whole project rests on: that *living inside this simulation is engaging on its own*. Everything past that is production. Everything before it is the only thing that matters.
