# Species Cognition — The Umwelt Model

> Status: **research + architecture proposal, awaiting agreement. No code.**
> Relationship to prior docs: this is the layer *beneath* everything written so far. `SPECIES_PERCEPTION.md` recorded the v1 overlay build (wrong abstraction) and the v2 vision-gating slice. `PERCEPTION_DESIGN.md` gave us "render knowledge, not world" and the knowledge ladder. `SPECIES_KNOWLEDGE_FOG.md` promoted knowledge to a spatial field and a core pillar. **This document reframes all three: the fog is not the system — it is one *renderer* of a deeper object, the per-animal belief model (its Umwelt). It also hardens the biology into cited, confidence-tagged numbers, and proposes the game's signature mechanic.**
> Scientific-honesty flag (per `CLAUDE.md`): every biological figure carries a confidence tag — **[established]** (replicated primary data), **[theory]** (leading framework, contested at edges), **[analogue-inferred]** (measured in a related species, extrapolated), **[folklore-corrected]** (a popular claim corrected against the literature). Full citations in the appendices. Where the game departs from biology, it is flagged as a **[design liberty]**.
> Written wearing five hats at once, as requested: cognitive scientist, evolutionary biologist, neuroethologist, animal-behaviour researcher, game designer.

---

## 0. Executive summary — the whole argument on one page

1. **The reframe is correct, but "cognition" must be bounded.** We are not simulating thinking. We are simulating the **belief state that thinking runs on**: a per-animal, partial, uncertain, *aging* model of the world. Every species "thinks differently" not because we author reasoning per species, but because we **constrain what each species is capable of believing**. Cognition is an *emergent property of a constrained belief store*, not a reasoning engine. This is the single most important decision in the document, and it is what keeps the idea buildable by a solo dev.

2. **The scientific spine already has a name:** von Uexküll's **Umwelt** — the self-world an organism's senses and nervous system construct. Two animals in the same meadow inhabit different worlds. Switching species is not a re-skin of the view; it is a change of *which world exists for you*. `[established — foundational]`

3. **The biology, hardened.** Four species, ~20 sensory/cognitive dimensions each, cited and confidence-tagged. Several earlier numbers were folklore and are corrected here (§2, appendices).

4. **The cross-species discovery (the heart, §3):** each species perceives a different **depth of time** off the same ground. Deer = the present instant. Cat = a sharp, deep, *forward* present. Wolf = hours into the past. Human = the past *plus a projected future*. Your own four native questions are four points on one time axis. Time is the hidden dimension of perception, and no survival game has ever made it the axis.

5. **The architecture (§5)** answers your seven questions with four objects: **Stimulus Field** (the world's own decaying record) → **Channels** (species-specific sampling rules) → **Umwelt / belief store** (typed, aged, uncertain percepts) → **Consumers** (AI brain reads it as data; player screen renders it as the Knowledge Fog). One model, two consumers — the possession invariant (D7) extended to the senses.

6. **The signature mechanic (§6):** **"the world remembers, and every sense reads its memory at a different age."** The ground and air hold a decaying, honest record of everything that moved through; your species decides how far back — and whether forward — you can read it. It is the one mechanic no stealth, horror, or survival game can copy without becoming this game.

7. **Scope discipline (§7):** no reasoning engine, no emotion simulation, no learning, no per-pixel sensing. The belief store *is* the anti-overengineering move. Validate the smallest slice first (§8).

---

## 1. The reframe: eyesight → perception → cognition — and exactly where to stop

### The trajectory so far was right
- **v1 (overlays):** rendered the true world to everyone, decorated it per species. An omniscient observer wearing filters. Switching species never changed a decision. Wrong.
- **v2 (knowledge-gating):** the screen renders what the body *knows*, not the world. An unsensed animal is *absent*, not dimmed. Correct direction.
- **Knowledge Fog:** knowledge is not only per-entity but per-*place* — a spatial field of "what this body knows here, and how stale it is." Correct escalation.

Each step subtracted the player's omniscience and pushed knowledge closer to the body. The next step is to name what the fog is *drawing*.

### The challenge you asked me to make: "cognition" is too broad as a build target
You proposed simulating cognition rather than eyesight, and asked me to push back if cognition is too broad. **It is — as an engineering mandate.** "Simulate cognition" literally means simulate perception + attention + memory + reasoning + emotion + learning + prediction. That is an unbounded research programme, not a feature. It is precisely the "more systems" scope-creep that `VISION.md` names as the #1 project-killer. If we chase it head-on we will build a shallow, buggy fraction of a mind and call it a game.

### The disciplined abstraction that preserves the same emotion
Do not simulate the mind. Simulate **the thing the mind operates on**: a **belief state** — one animal's private, partial, *aging*, uncertain model of the world.

> A belief is a small record: **{ what (a feature-set), where (a position or region), how sure (confidence 0–1), how old (age), from which sense (source channel) }**.

That is the entire ontology. And here is the move that makes "every species thinks differently" cheap instead of impossible:

**We do not write per-species reasoning. We constrain, per species, what beliefs can even *form*.** A deer's belief store is physically incapable of holding "wolf, at (x,y)" — its channels can only ever write "something changed, that direction, low confidence." A wolf's store *can* hold "canine-ish, passed here, ~12 minutes ago, heading north-east" because its scent channel writes exactly that shape of belief. The "different cognition" is not authored behaviour; it is **the different shape of what is believable**. The thinking emerges from the constraint.

This is `[design]` grounded in `[established theory]`: it is von Uexküll's Umwelt expressed as a data structure, and it is the same model–view split that MVC uses and that the project already committed to for possession (one AI-driven agent, two ways to drive it → one belief store, two ways to consume it).

**What we gain by stopping here:** the belief store is finite, testable, server-computable, and shared by AI and player. "Different game per species" becomes a *data* property (which channels, what they write, how fast it decays) rather than four hand-built cognition systems. We get the emotion of inhabiting a different mind without simulating a mind.

---

## 2. The scientific foundation (trustworthy numbers)

Full cited briefs are in Appendices A–E. Two corrections to numbers the earlier docs carried, because they matter for trust:

- **"Wolves/dogs smell 10,000×–100,000× better than humans" is a slogan, not a measurement.** Sensitivity gains are *odorant-specific*, ranging ~10³–10⁵ depending on the compound; humans even out-detect dogs on some odorants. Cite odorant-specific thresholds, never a universal multiplier. `[folklore-corrected]` (McGann 2017; Concha 2019.)
- **"Deer can't see blaze orange" is wrong**, and **there is no measured deer flicker-fusion figure.** Deer are dichromats who see orange as muted grey-brown (not invisible); the real detectability issue is UV-brightened fabric glowing. Do not hard-code a deer CFF in Hz — it is unmeasured. `[folklore-corrected]` (VerCauteren & Pipas 2003; Healy 2013.)

### Cognitive profiles — framed as cognition, not sensor specs

This table deliberately does **not** repeat FOV/acuity numbers (those live in `SPECIES_KNOWLEDGE_FOG.md` §2 and Appendices A–D). It answers your actual questions: what each species ignores, cannot perceive, treats as urgent, remembers, and how much uncertainty it lives inside.

| Cognitive dimension | **Deer** (prey) | **Wolf** (courser) | **Big cat** (ambusher) | **Human** (persistence hunter) |
|---|---|---|---|---|
| **What it ignores** | stationary shapes; identity; detail | fine visual detail; static scenes | anything not in the forward cone; the past | its own periphery while focused (inattentional blindness) |
| **What is impossible for it to perceive** | *what* a motionless thing is | a scene's fine colour/detail at rest | scent history; anything behind it; distant time | scent-trail age/direction; UV; ultrasound; the dark |
| **What is immediately urgent** | *any change* in the periphery | a fresh scent crossing the wind | motion inside the forward cone within lunge range | a track, a drop of blood, a projected intercept |
| **What it remembers** | almost nothing; re-alarms on the same thing | a landscape of marks + trails (shared, persistent) | a locked target through brief occlusion | kilometres of terrain + a specific individual for hours |
| **What decays fastest** | everything (fast, jumpy) | live positions (but scent-past lingers) | everything outside the lock | least — slow memory is the human superpower |
| **What triggers instinct (reflex, pre-thought)** | motion onset → freeze/flush | fresh downwind scent → orient/approach | prey bolting → pounce commit | (fewest reflexes; most deliberation) |
| **How much uncertainty it lives inside** | enormous, and usually *unresolvable* | moderate about *now*, low about *the recent past* | near-zero forward, total everywhere else | high about the present, low about *the inferred future* |
| **Native question** | *"What moved?"* | *"Who passed here, and when?"* | *"Can I close the gap before it runs?"* | *"Where is it going next?"* |

### The unifying frames (both scientifically load-bearing)

- **Umwelt (von Uexküll 1934).** No shared objective world — only overlapping species-worlds. This is the design's spine and its scientific honesty in one concept. `[established]`
- **Critical flicker-fusion / temporal resolution (Healy et al. 2013, *Animal Behaviour*).** How finely an animal samples *time* — whether the world looks smooth or strobed, and how early it can register the *onset* of a movement. Crucially, the folklore "predators see fast, prey see slow" is **backwards-ish**: CFF is predicted by body size and metabolism, not trophic role. A big cat sits at an unremarkable ~55 Hz; small fast prey can genuinely register the first frame of a lunge a beat sooner. `[established finding; the inversion is a design gift]`

---

## 3. The cross-species question: what each species knows that the others cannot

This is the section you said matters most. The answer is not a list of four sensory edges. It is a single realisation:

> **Each species reads a different depth of *time* off the same physical world.**

Perception is usually treated as a spatial question ("how far, how wide, how sharp"). The biology says it is also — maybe primarily — a *temporal* question. Every event in the world leaves records that decay at different rates: a moving body displaces air *now*, makes a sound that dies in *seconds*, bends grass and drops scent that lingers for *minutes to hours*, and leaves tracks that last *hours to days*. A sense is not just a window onto space; it is a window onto a **particular age of the past**.

- **The deer reads the razor-thin present.** Its whole system is a panoramic change-detector: *what is different this instant?* It cannot read the past (poor scent-history) and cannot project a future (no tracking cognition). It lives at time ≈ 0, everywhere at once, at terrible resolution — and its native tragedy is that it usually cannot *resolve* the change it detects before it must decide to flee. Uncertainty is its permanent home. `[established sensory basis; theory framing]`

- **The cat reads a sharp, deep, *forward* present.** Time ≈ 0 again, but the opposite geometry: a narrow forward cone with superb depth and motion, and near-nothing elsewhere or elsewhen. It owns the *now* in front of it so completely that the rest of the world — behind it, and any other moment — barely exists. Its clock is seconds long: it has no answer once the present becomes the past (no endurance, no trail-reading). `[established geometry + energetics]`

- **The wolf reads *hours into the past*, across the ground.** This is the genuinely alien one. From the concentration gradient across as few as ~5 footprints a canid infers **direction of travel**, and from volatility decay it reads **age** down to seconds of difference (Hepper & Wells 2005). It stands on empty ground and knows *an animal passed here, this way, this long ago* — a fourth dimension laid over a scene the others see only in the present tense. It also reads a persistent *social* layer (scent-marks: who owns this, how fresh), maintained by the whole pack over time. The wolf natively **time-travels backward through the terrain.** `[established mechanism in dog; analogue-inferred for wolf]`

- **The human reads the past *and projects the future*.** The human senses *less* than all three on almost every channel — but it is the only species that turns a static trace into a moving model and then runs that model *forward*: a footprint becomes a whole animal (species, gait, health, how long ago), and then a *prediction* of where it must have gone, tested against fresh sign (Liebenberg's tracking-as-the-origin-of-science; the predictive-brain framework, Clark/Friston). Object permanence lets it keep reasoning about a quarry it cannot sense. Its one true sensory edge — trichromatic colour — pays off only through inference (that red smear = a gut wound = it will tire = head for water). The human is the only animal whose perception points at time > 0, the *future*. `[established + theory]`

**So the four native questions collapse into one axis:**

```
         PAST  ◄───────────────── NOW ─────────────────►  FUTURE
                                    │
   WOLF: "who passed              DEER: "what               HUMAN: "where
   here, and when?"               moved?" (widest,          is it going
   (reads hours back              shallowest present)       next?" (the only
   off the ground)                                          forward-reader)
                                  CAT: "can I reach it
                                  before it runs?"
                                  (sharp, deep, forward,
                                   present-only)
```

No other survival or animal game has made **temporal depth of perception** its organising idea. That is the opening §6 exploits.

---

## 4. Games research: principles for building incomplete information

Twelve principles distilled from stealth, horror, immersive sims, RTS/tactical, deduction, and animal sims (full analysis in Appendix E). The five that most shape *this* game:

1. **Direction-without-identity is the strongest fear signal available.** Bearing but not identity, presence but not position, age but not author — the mind completes the picture with the worst case (Alien: Isolation's motion tracker; Silent Hill's radio). *For an animal this is native:* a smell says "predator, recent, close" long before it says which or where.
2. **Render other minds' belief states, and make the gap between their model and reality the play space** (MGS's "?/!", Thief's guard barks). *We get this for free:* our AI animals run on the same belief store, so a wolf genuinely searches your *last-known* position while you flank — and the player can read that it's doing so.
3. **Traces are frozen narrative — hand the player evidence, never conclusions** (Return of the Obra Dinn). *This is the anti-WolfQuest rule:* never render scent as labelled trails that announce "elk, 2 hours." Present age/direction/arousal; let the *player's* cognition assemble "a stressed deer ran through here ahead of something heavier."
4. **Attention is a spendable, exclusive resource with a footprint** (Isolation defocuses the world while you read the tracker; sonar/beep *emit*). Focusing one channel must cost the others; active sensing should announce you.
5. **Three-state knowledge — unknown / remembered / live — and render the *decay* of the remembered.** RTS fog's "grey" is memory drawn as fact; its unclaimed improvement is that grey never visually *ages*. Our fog must let confidence drain out of the rendering itself (detail loss, desaturation, positional smear) so the player *feels* staleness.

And the two hard rules that gate everything: **never fabricate** (honest-vague, never a false positive — the trust contract), and **never ship the omniscience tool** (a free, always-correct sense — eagle vision, Dishonored's Dark Vision — teaches players to stop reading the world; they take the deal and resent you for it).

---

## 5. The Cognitive Architecture

Four objects. Everything you asked about is a question about one of them.

```
   ┌─────────────────────────────────────────────────────────────────┐
   │  WORLD (ground truth — the simulation)                           │
   │   movers, injuries, feeding, weather, wind vector                 │
   └───────────────┬─────────────────────────────────────────────────┘
                   │ actors continuously emit stimuli
                   ▼
   ┌─────────────────────────────────────────────────────────────────┐
   │  ① STIMULUS FIELD — "the world's own memory"                     │
   │   append-and-decay records placed in the world, species-agnostic:│
   │   motion pulses · sound events · scent deposits (w/ direction+age)│
   │   · tracks/disturbance · blood.  Each has position, features,     │
   │   a timestamp, a decay rate.  Wind reshapes scent/sound.          │
   │   (UScentFieldSubsystem is the seed of this — generalise it.)     │
   └───────────────┬─────────────────────────────────────────────────┘
                   │ sampled by
                   ▼
   ┌─────────────────────────────────────────────────────────────────┐
   │  ② CHANNELS — species-specific sampling rules                    │
   │   {geometry, feature-filter, acuity, latency-window, noise}.      │
   │   Vision reads live actors at age≈0 → identity+position.          │
   │   Motion reads change at age≈0 → direction only.                  │
   │   Scent reads the Stimulus Field at age minutes→hours → who-ish + │
   │   direction + recency.  Sound → source region + kind.  A species  │
   │   is just a weighted SET of these.  Nothing hardcoded per species.│
   └───────────────┬─────────────────────────────────────────────────┘
                   │ each channel writes typed percepts into
                   ▼
   ┌─────────────────────────────────────────────────────────────────┐
   │  ③ UMWELT — the per-agent belief store                          │
   │   • Entity ledger: per-entity rung (Absent/Detected/Classified/   │
   │     Tracked/Remembered) + confidence + age + source.              │
   │   • Spatial fog: coarse grid of "knownness" + freshness per cell. │
   │   Confidence DECAYS toward zero at a species/channel half-life.   │
   │   Never fabricates: absence is absence; vagueness is honest.      │
   └───────────────┬─────────────────────────────────────────────────┘
                   │ ONE model, TWO consumers (D7)
        ┌──────────┴───────────┐
        ▼                      ▼
   ④ AI BRAIN             ④ PLAYER RENDERERS
   reads Umwelt as        the Knowledge Fog + entity rendering +
   decision input;        scent/track layer + memory-ghosts.
   acts only on its       The screen is a render of the Umwelt,
   beliefs, never on      NEVER of the world.  (Debug renderer
   world-truth.           draws ground truth for US only.)
```

**Your seven questions, answered against this diagram:**

1. **How does the world generate information?** Actors emit **stimuli** into the **Stimulus Field** as a side effect of existing: moving drops scent + bends grass + displaces air + (if fast) makes sound; being injured drops blood. The field is the world's own append-and-decay record — species-agnostic ground truth about *what happened where, and how long ago*. This is the object that makes "the world remembers" (§6) real.

2. **How do senses acquire information?** Each **channel** is a sampling rule with a *latency window* — the age of stimulus it can read. Vision/motion read age≈0; sound reads seconds; scent/tracks read minutes-to-hours. A channel filters features (vision→identity, motion→direction only, scent→who-ish+recency+heading), applies acuity (resolution/certainty), and adds noise. It writes percepts into the Umwelt. A species = a weighted set of channels. **This is where temporal depth (§3) becomes mechanical.**

3. **How is uncertainty represented?** Every belief carries **confidence (0–1)** and a **rung** (coarseness of the feature-set). Uncertainty = low confidence and/or a low rung ("something, there" vs "wolf, at (x,y)"). It is never a randomised or fabricated value — a coarse percept *claims only what it actually knows*. Vagueness is a truthful statement of ignorance, never a lie.

4. **How does knowledge decay?** Two independent decays. (a) The **Stimulus Field** decays — scent fades, tracks weather away — so the *world's* memory is finite. (b) **Belief confidence** decays toward zero at a species/channel half-life, dropping percepts down the rungs (Tracked → Remembered → Absent). The human's slow belief-decay is a superpower; the deer's fast, jumpy decay is a curse.

5. **How does memory work?** Memory is simply a belief no live channel is refreshing — a percept whose *age keeps growing* while its confidence falls. "Remembered" is a first-class rung: a static ghost at last-known state that **does not update**. Species differ here and *only* here for memory: the human's ghost decays slowly and can be **projected forward** by its last-known heading (a forecast, explicitly flagged as uncertain — the persistence-hunter's edge); the deer's ghost is short-lived and re-alarms.

6. **How do AI agents consume it?** The **AI brain reads the Umwelt as its decision input and can act on nothing else.** A hunting wolf pursues where it *believes* you are — your decaying last-known position — not where you truly are. This is what makes the animals believable (the Rain World / Alien-Isolation "honest imperfect senses" principle) *and* makes the hunt fair: predator and prey reason from the same kind of partial, aging belief. It also keeps everything server-authoritative and per-agent — the multiplayer property stays free.

7. **How does the player consume it?** Through **renderers that draw the Umwelt, never the world.** The **Knowledge Fog is one such renderer** — it draws the spatial "knownness" grid. Entity rendering draws known actors *at their rung* (Tracked = real actor; Remembered = fading ghost; Detected = ambiguous proxy — never an icon). The scent/track layer draws the readable Stimulus Field for nosed species. A debug renderer draws ground truth for *us*. The player is the sensor-fusion layer; that fusion is the cognition gameplay.

### Where this leaves the Knowledge Fog

The fog is **demoted from "the system" to "the primary player-facing renderer of the belief store."** Nothing in `SPECIES_KNOWLEDGE_FOG.md` is wasted — the grid, the decay, the rungs, the no-fabrication rule all survive verbatim. They just move *underneath* the fog, into the Umwelt, where the AI can share them. The fog remains the make-or-break rendering problem (legibility of absence), but it is now correctly scoped as a *view*, and a second view (the AI) validates the model independently.

---

## 6. The signature mechanic

**Proposal: "The world remembers, and every sense reads its memory at a different age."**

The ground and the air hold a physical, decaying, *honest* record of everything that has moved through — and your species determines how far back you can read it, and whether you can read *forward*. Vision reads the record at age zero. Sound reads it seconds old. Scent and tracks read it minutes to hours old. Memory reads the record *you* wrote, aging in your own head. And the human, alone, reads the record forward — projecting where the trail leads before arriving.

**Why this and not something merely interesting:**

- **It is genuinely new.** Obra Dinn proved reconstructing the past from traces is the strongest cognitive emotion in games — but with hand-authored, one-shot content. RTS fog proved players will reason about stale spatial memory — but only as hygiene, never the core verb. *No game has shipped a world where the past is physically present as a decaying, readable field generated by a live ecosystem.* Perceiving **time as a layer of space** is a perceptual fantasy no player has had. (Appendix E's "spine" argument.)
- **It falls out of the pillar; it is not bolted on.** It is a direct, honest consequence of the Umwelt architecture (§5) and the temporal-depth biology (§3). We are not inventing a gimmick — we are surfacing the deepest true thing about how these animals actually perceive.
- **It is inseparable and un-copyable.** A stealth or horror game can borrow "direction without identity." It cannot borrow "every species reads a different depth of time off a living world's memory" without rebuilding this entire simulation. That is what makes it an *identity*, not a feature.
- **It subsumes the prize principles instead of competing with them.** Traces are direction-without-identity by nature (P1); reading a trail is reconstructing another mind's state (P2); sniffing is attention spent while exposed (P4); trace confidence decays like fog memory (P5); and because traces come from an indifferent live simulation, the Obra-Dinn one-shot ceiling disappears — *every hunt is a fresh crime scene with no witnesses* (P9).

**Honest caveats — where I will not oversell it:**
- The past-reading pole is expressed most strongly by the **wolf and human**; the **deer and cat** sit at the shallow-present end of the *same axis*. That is a feature (one axis, species at different points), but it means the mechanic's *fullest* expression is a wolf/human thing — and the first playable is the human. Good: the human's forward-projection (tracking a fleeing animal by inferring its route) is the most *legible* version to a new player, and it is exactly `PROTOTYPE_SPEC.md`'s persistence-hunt loop.
- "Signature mechanic" is a phrase that invites feature-fetish. The signature must be *proven small* before we crown it. If reading the world's memory is not fun with one human and one fleeing animal on a greybox, it is not the signature no matter how elegant the theory.

**The one-line identity:** *You are the detective of a world with no witnesses, and the mystery is written on the ground — but which chapters you can read depends on the animal you are.*

---

## 7. Scope discipline — what this architecture forbids

The belief store is chosen *because* it is the anti-overengineering move. To keep it that way:

- **No reasoning engine.** We never author "the deer deduces X." Deer cognition is the *shape of its belief store*, nothing more. Emergent, not scripted.
- **No emotion simulation.** "Fear" is not a modelled affect; it is a small set of **instinct triggers** — reflex rules that fire on a belief pattern (motion-onset in periphery → flush). A handful of triggers, not an emotion system.
- **No learning / memory of habits** (the Alien-Isolation "it learns your tricks" layer). Tempting, out of scope until the base model is proven fun.
- **No per-pixel / per-blade sensing.** Coarse grid + per-agent ledger + a bounded Stimulus Field. Prototype scale is ≤ a dozen agents. The design must never assume otherwise.
- **No new HUD.** Percepts render diegetically (in-world proxies, ghosts, trails), never as icons or meters. The only permissible meters are for the player's *own body* (stamina, already exists).
- **One Stimulus Field feature at a time.** Scent is the richest and most tempting channel; it does not enter until vision, motion, and memory are proven (§8).

---

## 8. Smallest validation path (after sign-off — not started)

Each step is independently playable and testable. This mirrors and *supersedes* the build orders in the two perception docs by routing everything through the Umwelt.

1. **Umwelt + Visual channel + fog grid.** The belief store exists; the Visual channel (generalise the slice-1 gating) writes entity rungs and the spatial knownness grid; unknown cells veil. *Test:* human vs. cat feel like different creatures with nothing else built (proves legibility-of-absence, the scariest risk).
2. **Belief decay + Remembered ghosts.** Confidence fades over a species half-life; last-known entities render as aging ghosts. *Test:* the human's long memory vs. the cat's short one appears; can players *feel* staleness in the rendering (P5)?
3. **AI reads the Umwelt.** Point one wander-brain's decisions at its *own* belief store instead of world-truth. *Test:* does an animal visibly search your last-known position while you slip away (P2)? This validates "one model, two consumers" cheaply and is the strongest believability payoff for the least new art.
4. **Motion channel + Detected proxies.** Wide motion entry, ambiguous in-world proxies. *Test:* the deer becomes real; the stationary-predator dilemma exists; "what moved?" is playable.
5. **Stimulus Field + Scent channel + wind.** Re-route the scent subsystem into the Stimulus Field; the wolf reads age + direction as *evidence*, never as labels. *Test:* can a player narrate the past from the ground (P3)? This is the signature mechanic's first real proof.

**Headline success condition (unchanged):** testers describe switching species as *"I have to think differently,"* not *"the screen looks different"* — and, for the signature, a player who has never been told how scent works stands on empty ground and says *"something came through here, recently, heading that way."*

---

## Appendices — cited research corpus

> Confidence tags as in the header. These are the trustworthy numbers; the design sections above draw on them but the citations live here. Load-bearing citations were verified against primary venues during the research pass; sensory *numbers* without a named study are standard textbook physiology (ranges given where sources vary).

### Appendix A — Gray wolf (*Canis lupus*); dog as analogue where flagged
- **Vision:** FOV ~250–270°, binocular overlap ~30–60° `[analogue-inferred]` (Miller & Murphy 1995, *JAVMA* 207:1623–1634). Acuity ~20/50–20/75, ~4–12 cyc/deg — roughly 1/3–1/6 of human detail `[analogue-inferred]` (Odom et al. 1983, *IOVS*). Flicker-fusion ~70–80 Hz (cone), higher than human `[analogue-inferred]` (Byosiere et al. 2018, *Anim Cogn*). Dichromat, S-cone ~429–435 nm / L-cone ~555 nm `[established, dog]` (Neitz, Geist & Jacobs 1989, *Visual Neuroscience* 3:119–125). Strong night vision: rod-dominated + tapetum `[established]` (Beltran et al. 2014, *PLOS ONE*).
- **Olfaction (primary sense):** ~800–1,000 functional OR genes `[established]` (Quignon et al. 2003/05, *Genome Biology*). Field prey-scent detection up to ~2.4 km in favourable wind; scent was the primary detection channel over sight/tracks `[established]` (Mech 1966; Mech & Boitani 2003). **Trail direction from as few as ~5 footprints, failing at 3; age discriminable down to ~1–2 s of print aging** — the core "reads the past" evidence `[established, dog; analogue-inferred, wolf]` (Hepper & Wells 2005, *Chemical Senses* 30:291–298).
- **Wind:** wolves chose downwind (olfactory-concealed) ambush sites **~89–94%** of the time `[established]` (Gable et al. 2021, *Behavioral Ecology* / Voyageurs Wolf Project).
- **Cognition:** scent-marks encode identity/status/ownership/freshness; edge-marking ~2.1× center `[established]` (Peters & Mech 1975, *American Scientist* 63:628–637). Pack hunting is largely emergent from simple individual rules `[theory]` (Muro et al. 2011, *Behavioural Processes* 88:192–197). Prey selection targets the vulnerable; large-prey killing is learned skill `[established]` (MacNulty et al. 2009, *Ecology Letters*). Hearing ~65 Hz–45 kHz `[established, dog]` (Heffner 1983).
- **Caveat:** nearly all quantitative sensory values are dog-measured, wolf-assumed; "mental model" is a defensible interpretation, its *components* (direction, age, identity) are the experimentally grounded parts.

### Appendix B — Cervids (red deer *Cervus elaphus*, reindeer *Rangifer tarandus*)
- **Vision:** FOV ~300–310°, binocular ~50–65° `[established WTD; analogue-inferred red deer/reindeer]` (Heffelfinger 2006). Acuity poor — use the **range ~20/60–20/200**, not a fixed 20/100 `[theory]`. Horizontal **visual streak**, not a fovea — resolution concentrated along the horizon `[established]` (D'Angelo et al. 2008, *Can. J. Zool.* 86:735–743). Dichromat, S ~450–460 nm / M ~537 nm, poor long-wavelength `[established]` (Jacobs, Deegan, Neitz & Neitz 1994, *J. Comp. Physiol. A* 174:551–557). **No measured deer CFF — do not hard-code one** `[folklore-corrected]`.
- **Reindeer UV:** ocular media transmit to ~320 nm; against UV-bright snow, wolf fur / urine / lichen appear as **dark signals** — an information band predators lack `[established physiology; theory on behavioural use]` (Hogg et al. 2011, *J. Exp. Biol.* 214:2014–2019). Reindeer-specific; do **not** generalise UV to other deer.
- **Hearing:** ~0.25–30 kHz, into ultrasound above the human ceiling; independently rotating pinnae `[established]` (D'Angelo et al. 2007, *J. Wildl. Manage.* 71:1238–1242).
- **Behaviour:** head-up/head-down vigilance–foraging tradeoff; per-capita vigilance falls with group size (measured by blink rate) `[established]` (Rowe et al. 2023, *Ecol. Evol.* 13:e9908; Beauchamp 2008). FID is a context-dependent decision surface, **not** a constant `[established]`. Poor at identifying motionless objects → the ambush gap; freeze-and-scan, downwind circling to force a cue `[theory]`.
- **Olfaction:** primary trusted threat sense, wind-vectored; **the popular distance/receptor numbers are unmeasured folklore** `[folklore-corrected]`.

### Appendix C — *Homo sapiens* (perception + cognition)
- **Vision:** FOV ~200–220°, binocular ~114–120°; foveal acuity ~1 arcmin but the fovea is only ~1.5–2° — detail is a tiny moving spotlight, the seamless world is a *construction* `[established]`. Rapid object-ID: "animal / not" in ~150 ms (Thorpe et al. 1996, *Nature* 381:520–522); biological-motion reading from sparse cues (Johansson 1973) `[established]`. **Trichromacy — best mammalian colour**; the red-green channel sees blood spoor / flushed skin / vegetation change the dichromat animals cannot resolve `[established]`. Night vision poor, no tapetum `[established]`.
- **Olfaction:** the "humans smell badly" belief is a 19th-century myth; ~400 functional OR genes, humans out-detect dogs on some odorants `[established]` (McGann 2017, *Science* 356:eaam7263). Humans can nose-track a ground trail, using inter-nostril comparison `[established]` (Porter et al. 2007, *Nat. Neurosci.* 10:27–29). Still far below canids for trail work — keep it asymmetric.
- **Cognition (the superpower):** predictive brain — perception as the brain's best hypothesis, forecasting where things *will be* `[theory]` (Clark 2013, *BBS* 36:181–204; Friston 2010, *Nat. Rev. Neurosci.*). Object permanence → keeps reasoning about the unseen quarry `[established]`. **Tracking as hypothetico-deductive inference** — reconstruct species/gait/health/heading from static sign, then *speculatively* predict where it went `[established/theory]` (Liebenberg, *The Art of Tracking*, 1990; 2006, *Current Anthropology* 47:1017–1026). Working memory ~4 chunks (Cowan 2001) but vast long-term spatial maps `[established]`. Attention is a spotlight that fails: inattentional blindness (Simons & Chabris 1999, *Perception* 28:1059–1074), change blindness `[established]`. Reaction time slow (~200 ms visual) — the human is a poor reactor, a superb predictor.
- **Persistence hunting:** endurance-running adaptations + sweat thermoregulation drive prey to hyperthermic collapse over ~2–5 h / 25–35 km, requiring continuous tracking of one chosen individual `[theory-strong + observational]` (Bramble & Lieberman 2004, *Nature* 432:345–352; Liebenberg 2006/2008).
- **Thesis:** *Give the animals the better senses; give the human the better model.*

### Appendix D — Big cat (lion/tiger; *Smilodon* placeholder) + temporal frame
- **Vision:** FOV ~200°, binocular overlap ~120–140° (ambush depth) `[established, cat]`; *Smilodon* orbital convergence ~63.9° confirms forward-facing eyes in bone `[established]` (Comm. Biol. 2023) — stereopsis itself `[analogue-inferred]`. Daylight acuity poor (~20/100–20/200) `[established]`. CFF ~53–57 Hz — *not* a fast-temporal animal `[established]` (Vision Research 1980). Night vision ~6–8× human via tapetum + rods `[established, cat; analogue-inferred Smilodon]`.
- **Hearing:** ~48 Hz–85 kHz, deep into ultrasound; pinnae localise to ~7.6 cm at 1 m `[established]` (Heffner & Heffner 1985).
- **Energetics/behaviour:** stalk + explosive anaerobic sprint ~20–150 m, exhausts in seconds; success ~20–25%; target fixation; weak rear/peripheral awareness `[established]`. **Inverse of the wolf's endurance economy.** Native question: *"Can I close the gap before it runs?"*
- **Temporal-resolution frame:** CFF predicted by body size + metabolism, not trophic role — small fast prey can out-sample time and register motion-onset first (Healy et al. 2013, *Animal Behaviour* 86:685–696) `[established]`. The "predators see fast" trope is overstated `[folklore-corrected]`.
- **Umwelt (spine):** each organism inhabits a self-world of only what its senses register; no shared objective world (von Uexküll 1934/2010) `[established]`. Senses as information channels with distinct geometry/latency/noise (Dusenbery 1992; Stevens 2013).

### Appendix E — Cross-genre design principles (condensed)
The twelve principles and the "spine" argument for reconstructing-the-invisible-from-traces are summarised in §4 and §6. Key exemplars: **Alien: Isolation** (direction-without-identity; two-brain honest-imperfect AI), **Metal Gear Solid** (belief-state-as-game-state; last-known-position play), **Return of the Obra Dinn** (traces as frozen narrative; evidence-never-conclusions), **Darkest Dungeon** (sell certainty for reward; chosen darkness), **RTS fog** (three-state knowledge; unrendered decay as the unclaimed improvement), **Darkwood / Project Zomboid** (isometric fog-of-perception proven; occupancy hidden under visible geometry), **Rain World** (indifferent ecosystem; creatures with honest senses and agendas), **Dishonored Dark Vision / AC eagle vision** (the omniscience anti-pattern — never ship it), **WolfQuest** (the anti-pattern for scent — labelled trails delete the player's cognition). The strongest single, unexploited idea: **make the world's readable, decaying memory the spine** — a systemic, spatial, continuous version of Obra Dinn's reconstruction, which no other genre can copy without becoming this game.
