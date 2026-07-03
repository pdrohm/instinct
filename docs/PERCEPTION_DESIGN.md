# Species Perception — Design Proposal v2

> Status: **proposal, awaiting agreement.** No code until this is settled.
> Supersedes the framing of the v1 debug prototype (`SPECIES_PERCEPTION.md`), which remains useful as scaffolding and as a record of the wrong abstraction.
> Governing question (H17, Q18, Q19): can different species experience the same world differently — as *different nervous systems*, not different HUD skins?

## 1. What v1 got wrong, precisely

V1 renders the true world to everyone and then *annotates* it per species: cones, rings, blobs, color grading. The player sees everything and receives species-flavored decoration on top. That is an **omniscient observer wearing filters** — the exact opposite of embodiment.

The tell: in v1, switching species never changes a decision. A deer player and a cat player standing in the same spot can both see the wolf behind them; one just has a prettier ring around it. Information availability is identical; only presentation differs. Perception that doesn't change decisions isn't perception — it's theme.

**The correct abstraction: perception controls what information exists for the player at all.** The screen should be a rendering of *this body's current knowledge*, not of the world.

## 2. First principles

### Render knowledge, not world

Between the world's true state and the player's screen sits one thing: the body's senses. Everything the player learns must pass through them.

```
world state → species sensory channels → knowledge model (beliefs, with uncertainty and age) → screen
```

The screen shows the knowledge model. An animal that no sense has registered **is not on the screen** — not dimmed, not desaturated: absent. This is subtraction, not decoration. The isometric camera (D14) still frames a wide view — but the entities in that view are knowledge-gated.

One principled exception: **terrain and static environment render fully.** Animals know their home range; withholding rocks would punish the player for the camera's vantage without adding meaning. The gate applies to *dynamic, living information* — agents, carcasses, disturbances. (This resolves Q18's core: static world = shown; living world = earned.)

### Every sense is a channel, and no channel carries everything

Information theory framing: a sense is a channel from world to knowledge, characterized by —

| Property | Meaning | Example contrast |
|---|---|---|
| **Geometry** | where it samples | vision cone vs. hearing sphere vs. upwind scent lobe |
| **Features carried** | what it can tell you | vision: identity+position. scent: identity+*history*, no present position. peripheral motion: direction+"something", no identity |
| **Acuity** | resolution/certainty of those features | human fovea vs. deer's blur |
| **Latency & persistence** | when the information is *from* | vision: now, gone when unseen. scent: minutes old, lingers |
| **Noise** | false alarms and misses | deer periphery fires on harmless movers; still things defeat it |

**A species is an allocation of a fixed information budget across channels.** Nobody gets everything — scarcity is the design contract. The human buys depth and inference by giving up width and smell; the deer buys width and motion by giving up identification; the wolf buys the past by accepting a vaguer present; the cat buys a devastating forward channel by going nearly dark everywhere else.

### Uncertainty is the product

The emotion of inhabiting a nervous system comes from its *failures*: the thing you can't identify, the sound you can't place, the trail that's three minutes stale. Certainty produces tactics; uncertainty produces feelings — fear, doubt, commitment, relief. Perception design is therefore mostly the design of **what kind of not-knowing each species lives inside**:

- Human: *"I know what it is, but I've lost where it is."* (narrow attention, strong memory/inference)
- Deer: *"I know where something is, but not what it is."* (wide detection, weak classification)
- Wolf: *"I know who passed here and when — but not where they are now."* (rich past, vague present)
- Big cat: *"I know everything about what's in front of me, and nothing else exists."* (depth without width)

One rule keeps uncertainty fair: **percepts never lie about their own claims.** A "something moved north-east" percept means something did move north-east. Vagueness is honest; randomness that fabricates would destroy trust and, with it, the willingness to act on partial information.

## 3. The knowledge ladder

Every dynamic entity, per observer, sits on one rung. Senses *enter* the ladder at different rungs — that asymmetry is where species identity lives.

| Rung | The body knows | Screen shows (direction, not final art) |
|---|---|---|
| **0 Absent** | nothing | nothing at all |
| **1 Detected** | *something* is in a direction/region; maybe "it moved" | an ambiguous presence at low fidelity: a shape-shiver in grass, a displaced silhouette, a sound source region — in-world, never an icon |
| **2 Classified** | what kind of thing; rough position or trajectory | recognizable but coarse: species-shaped, imprecisely placed |
| **3 Tracked** | identity + live position + state | the actual actor, full fidelity |
| **M Remembered** | where it *was*; decaying confidence | a static ghost of last-known state that fades; it does not update |

Sense entry points: human foveal vision enters at 3; deer peripheral motion enters at 1 (and *stationary* things may never leave 0 — the ambush interplay); wolf scent enters at 2-of-the-past (identity + freshness + travel direction, mapped onto Remembered rather than Tracked); hearing enters at 1 for everyone, with radius per species.

Memory (M) is a first-class rung, and species differ here too: the human's superpower is that M decays slowly and *predicts* ("it went behind the rise — it will emerge left"), which is persistence hunting's cognitive half. The deer's M is short and jumpy: it re-alarms on things it already saw.

## 4. Attention: perception must be a verb

Gating information is half the redesign. The other half: **perceiving must cost something**, or it's still passive. Each species gets one attention posture — a stance the player chooses, with an opportunity cost, tying perception into the existing energy economy (H2) rather than adding a new system:

- **Human — Focus.** Narrow the attention cone deliberately: things inside resolve to Tracked faster and further; everything else degrades *more*. Reading the land vs. watching your back is a choice.
- **Deer — Vigilance vs. grazing.** Head up: full peripheral entry, no feeding. Head down: feed (the future hunger loop), periphery dulled. This is *the* prey decision, and it needs no new mechanics — it is posture + the ladder.
- **Wolf — Nose down.** Read the scent layer richly (freshness, direction) while moving slowly with dulled vision; nose up to travel. Tracking becomes an activity, not an overlay.
- **Big cat — Lock.** Commit the forward channel to one target: it climbs to Tracked and *stays* there through brief occlusion; the rest of the world drops a rung. Power purchased with blindness.

Attention is the moment "different nervous system" becomes gameplay: the same three seconds — freeze, look, commit — play differently in every body.

## 5. One system, two consumers

The knowledge model is not player-only UI plumbing. **It is the same perception the AI brains need** (Stage 2 rival predators, fleeing herds). One pipeline — stimuli → channels → per-agent knowledge — with two consumers: an AI brain reads it as data; a possessed body renders it to a human. This is the possession architecture (D7) extended to senses, and it's the strongest reason to build it this way: we are not building "perception UI", we are building *the* perception system, which the player happens to inhabit. It also keeps the multiplayer property intact for free: knowledge is computed per-agent, server-side by construction.

## 6. Presentation principles (for later — principles, not techniques)

1. **Subtraction before addition.** The first implementation step is *hiding* things, not drawing new things.
2. **Uncertainty renders as vagueness in the world, never as UI.** A rung-1 percept is an ambiguous *thing at a place* — a shape, a rustle, a disturbance — not a ring or marker. If we can't afford the asset, a crude proxy shape beats an icon.
3. **Degraded ≠ useless.** Every rung must support a decision (investigate, avoid, stalk, flee). If a percept can't change behavior, cut it.
4. **The camera may not compensate.** No free information from the wide view: what the body doesn't know, the vantage doesn't show. (The v1 cones/rings survive only as a dev-only debug visualization toggle.)
5. **Trust is sacred.** Honest vagueness, no fabrication.

## 7. What survives from v1

- `USpeciesPerceptionProfile` — survives; fields reinterpret as channel parameters (entry rungs, ranges, acuities, memory rates) and grow an attention-posture block.
- `UScentFieldSubsystem` — survives nearly intact: it is already a stimulus source; only its *consumer* changes (knowledge model instead of direct drawing).
- Debug targets, switcher, HUD line — survive as test harness.
- The overlay drawing — demoted to a debug toggle. It was never the product; it becomes the tool for *us* to see what the knowledge model believes.

## 8. Validation: four litmus tests

The redesign passes when naive testers fail in species-correct ways on the same map:

1. **Human test:** something approaches from behind; the player genuinely doesn't know until they turn or hear. Do they start *checking their back* unprompted?
2. **Deer test (the stationary-predator test):** a motionless threat sits in plain view. Deer players should walk past it; the moment it moves, alarm. Do they describe the feeling as "I'm jumpy but half-blind"?
3. **Wolf test:** an animal the player never sees is findable by scent alone. Can they narrate the past ("something came through here, recently, heading that way")?
4. **Big cat test:** while locked on, a second animal flanks unseen. Do they describe the trade ("I had him — I never saw the other one")?

Plus the global test: players describe switching as *becoming something else*, not as changing view modes.

## 9. Risks

- **Frustration cliff.** Information hiding can read as unfair, especially at greybox fidelity where rung-1 proxies are crude. Mitigation: rungs must be generous at first (better slightly too knowable than opaque), and honesty rule enforced.
- **The proxy-asset problem.** Rendering "something ambiguous" diegetically is the hardest presentation task here and greybox may undersell it (same false-negative risk as H3). Accept crude proxies; judge the *decisions*, not the looks.
- **Attention postures creep toward full species gameplay.** Each posture is one input + parameter changes, nothing more. No new verbs beyond it.
- **Cost.** Per-agent knowledge bookkeeping is real engineering. Prototype scale (≤a dozen agents) is trivial; the design must not assume per-pixel or per-blade-of-grass sensing.

## 10. Smallest next slice (after agreement — not started)

1. Knowledge ladder for dynamic entities, vision-only, two rungs (Absent / Tracked): things outside the vision channel *disappear*. Human vs. big cat already feel different with nothing else built.
2. Add Detected + Remembered rungs with crude proxies (ghost = frozen grey duplicate; detected = generic shape). Deer becomes possible.
3. Reroute scent from drawing to knowledge (wolf reads Remembered-rungs from the scent layer).
4. One attention posture (deer's head-up/head-down is the cheapest and tests the deepest loop).

Each step is independently playable and testable against §8.
