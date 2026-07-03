# Species Knowledge Fog — Design Proposal

> Status: **proposal, awaiting agreement.** No code until this is settled.
> Relationship to prior docs: this is the spatial evolution of `PERCEPTION_DESIGN.md`. That doc gave us *per-entity* knowledge (the Absent→Tracked ladder). This doc adds the missing half: a *per-place* knowledge field — the fog — and promotes the whole thing from a perception feature to a **core gameplay pillar**. `SPECIES_PERCEPTION.md` remains the record of the v1 overlay build (the wrong abstraction) and slice-1 vision gating (right direction, small scope).
> Governs / touches: H8, H15, H17, Q18, Q19; pillar decision of 2026-07-03.
> Scientific-honesty flag: biological figures below are drawn from domain knowledge and cite real, established sources **by name**. Numbers marked ⚠ are approximate and must be verified against the primary source before they become tuning canon. This matches `CLAUDE.md`: distinguish known fact / theory / gameplay adaptation.

---

## 1. Design philosophy

### The fog is knowledge, not vision

Project Zomboid's fog is famous for one reason: it makes *not knowing* the core of the game. You do not fear the zombie you can see; you fear the street you cannot. But PZ's fog is a single human channel (line of sight over a 2D grid). Our world is 3D, isometric (D14), and multi-species — so our fog must be something PZ's never was:

**The fog represents what the inhabited body currently knows about the world — and every species knows the world through a different combination of senses, so every species sees a different fog over the same terrain.**

This reframes the earlier work. Overlays (v1) decorated an omniscient view. Entity-gating (slice 1) hid individual animals. The fog is the general system that contains both: it is a **spatial field of knowledge and its decay**, revealed through whichever sensory channels the current species possesses, and it never shows the player anything the body has not earned.

### Three commitments

1. **Reveal, don't just hide.** The fog is not darkness. It is the union of everything the body's senses currently deliver — and for a wolf that includes a scent trail through terrain it cannot see. Switching species doesn't dim the world; it swaps the *set of channels* that carve knowledge out of the fog.
2. **Knowledge decays.** What you knew a minute ago is not what you know now. A place you saw becomes *remembered* (a fading belief), then *unknown* again. Decay rate is a species trait — the human's superpower is that his decays slowly.
3. **Never fabricate.** Unknown stays unknown. Uncertainty is allowed and encouraged ("something moved, north-east"); a false positive that isn't real is forbidden. This is the trust contract that makes the player willing to act on partial information.

### The emotional target

The player should say **"I have to think differently"**, not "the screen looks different." The test of every mechanic below is whether it changes *how the player reasons about the unknown* — not how the frame is colored.

---

## 2. Research summary — sensory biology of modern equivalents

> Modern analogues stand in for Ice Age fauna: reindeer/red deer for the herd, gray wolf for the tracker, lion/tiger for the saber-tooth placeholder, and living humans for the early *Homo sapiens*. Saber-tooth (*Smilodon*) sensory biology is largely inferred from felid analogues and endocasts — flagged as speculation where it matters.

### Homo sapiens
- **Total horizontal field of view:** ~200–220°. **Binocular field:** ~114–120°. Monocular tails ~40° each side. Vertical ~130°. ⚠ (standard optometric values; Kaas and vision-science texts.)
- **Visual acuity:** ~1 arcminute foveal resolution (20/20). The fovea is only ~2° wide — high detail is a *tiny* central spot the eye moves around. Excellent distance identification among mammals.
- **Motion detection:** good but not exceptional; strongly foveal-biased. Peripheral motion sensitivity exists but resolves poorly.
- **Night vision:** poor. No tapetum lucidum; rod-limited; slow dark adaptation.
- **Color perception:** trichromatic — best mammalian color discrimination, strong at distinguishing objects against background.
- **Smell:** better than the folk myth. ~400 functional olfactory receptor genes; humans can track some scents. Still weak versus canids. (McGann 2017, *Science* — "Poor human olfaction is a 19th-century myth.")
- **Hearing:** ~20 Hz–20 kHz; good sound localization; no independent ear rotation.
- **Detection ranges:** identification-dominant — humans *recognize* things far away better than they *detect* them wide.
- **Behavioral adaptations:** endurance/persistence hunting (Bramble & Lieberman 2004, *Nature*; Carrier 1984, *Current Anthropology* — both already anchoring `PROTOTYPE_SPEC.md`), spatial memory, inference/pattern recognition, cognitive tracking (reading sign, predicting where prey went).

### Deer (reindeer / red deer, cervids)
- **Total horizontal field of view:** ~300–330° (laterally placed eyes). **Binocular overlap:** small, ~50–65° forward. Near-panoramic; a narrow blind wedge directly behind. ⚠ (VerCauteren & Pipas and cervid vision literature.)
- **Visual acuity:** poor — roughly 20/100 or worse; cannot resolve fine detail or reliably identify a still shape at distance.
- **Motion detection:** excellent, especially peripheral — the retina and wide field are built to catch movement anywhere.
- **Night vision:** very good — tapetum lucidum, rod-dominated retina. Reindeer specifically can see **ultraviolet** (Hogg et al. 2011, *J. Exp. Biol.*), making lichen and urine/tracks pop against snow.
- **Color perception:** dichromatic (blue–yellow); poor red/orange discrimination — the biological reason hunter blaze-orange is invisible to them.
- **Smell:** excellent; a primary threat-detection sense.
- **Hearing:** excellent; independently rotating pinnae for localization.
- **Detection ranges:** detects presence/motion very wide and far; *identifies* poorly — often cannot tell predator from rock until it moves.
- **Behavioral adaptations:** near-constant vigilance, freeze-and-scan, flush/flight-initiation distance, herd many-eyes effect, reliance on the collective alarm.

### Wolf (gray wolf, canid)
- **Total horizontal field of view:** ~250°. **Binocular:** ~60°. ⚠ (canid vision literature; Miller & Murphy on dog vision as analogue.)
- **Visual acuity:** poorer than human (~20/75 range); vision is a secondary sense.
- **Motion detection:** excellent.
- **Night vision:** excellent — tapetum lucidum, rod-dominated.
- **Color perception:** dichromatic (blue–yellow).
- **Smell:** extraordinary — on the order of hundreds of millions of olfactory receptors and ~800+ functional OR genes; can detect prey scent at long range, follow trails, and discriminate **trail age and direction**. Olfaction + hearing are the primary hunting senses.
- **Hearing:** excellent; sensitive well above human range (tens of kHz); detects distant sound across kilometers under good conditions.
- **Detection ranges:** scent detection dominates — a wolf routinely *knows something passed through* an area it never sees, and infers where it went and how long ago.
- **Behavioral adaptations:** scent-trailing, hunting into the wind (approach downwind), pack coordination via sound and scent-marking, long relentless pursuit.

### Big cat (lion/tiger; *Smilodon* placeholder)
- **Total horizontal field of view:** ~200°. **Binocular field:** ~100–120° — large forward overlap for depth, like all ambush predators. ⚠ (felid vision literature.)
- **Visual acuity:** good central detail, ~6× coarser than human in fine daylight detail but strong on movement; superb depth judgment forward.
- **Motion detection:** excellent within the forward field.
- **Night vision:** excellent — strong tapetum lucidum, rod-rich; several times more light-sensitive than humans.
- **Color perception:** dichromatic; color is largely irrelevant to the hunt.
- **Smell:** moderate — better than human, far below canid.
- **Hearing:** excellent; rotating pinnae; good high-frequency sensitivity for locating small prey.
- **Detection ranges:** short-to-mid, intensely forward. Weak rear/peripheral awareness — ambush predators are built to lock forward, not to watch their backs.
- **Behavioral adaptations:** stalk-and-ambush, explosive short pursuit, target fixation, binocular depth for the pounce. (*Smilodon*: powerful forelimbs and grapple-kill inferred from fossils — sensory specifics are analogue-based speculation.)

---

## 3. Perception channels

The fog is revealed by **channels**. A channel is a rule for turning world state into knowledge, with its own geometry, the feature it carries, its certainty, and how fast that knowledge decays. A species is a *set of channels with weights* — nothing more, nothing hardcoded per species.

| Channel | Geometry | What it reveals | Certainty | Decay |
|---|---|---|---|---|
| **Visual** | cone (FOV width × range), needs line of sight | identity + live position (high rung) | high, drops with distance/acuity | instant once out of sight → memory |
| **Motion** | wide arc / near-panoramic, needs LOS | "something moved, there" — direction, *not* identity | medium; only while moving | fast — a stopped thing vanishes from this channel |
| **Scent** | wind-shaped lobe + ground trails, ignores LOS | that something passed, who-ish, how long ago, travel direction | medium, degrades with age | slow — the past made perceptible |
| **Sound** | radius (species range), partial occlusion | a source region + rough kind (footfall, call) | low-medium; direction, not identity | fast — tied to the event |
| **Memory** | anywhere previously known | last-known state, *not updated* | decays from whatever rung it entered at | species-defined half-life |
| **Tracks / disturbance** | ground marks where bodies passed | a trail, aging | low-medium | slow (weather-driven later) |
| **Blood** | point/'splashes' near an injury event | strong "something was hurt here" | high locally | slow |
| **Wind** | global vector | *modifier*, not a percept — gates scent/sound reach and direction | n/a | changes over time |

Key design point: **channels carry different features.** Vision gives identity; motion gives only direction; scent gives history. This is what makes species *think* differently — a wolf reasons from history, a deer from movement, a human from identification and memory. Wind is not a channel you see; it is the rule that decides what the scent and sound channels can deliver, so it silently reshapes the fog.

---

## 4. Species comparison table (biology → channel configuration)

| | Homo sapiens | Deer | Wolf | Big cat |
|---|---|---|---|---|
| Visual FOV (game) | ~180° medium | ~300° panoramic | ~250° wide | ~90–100° narrow |
| Visual acuity / ID | **excellent** at range | poor — can't ID still shapes | mediocre | excellent, forward only |
| Motion channel | modest | **dominant, panoramic** | strong | strong, forward only |
| Scent channel | negligible | moderate (threat smell) | **dominant, trail + age + wind** | weak |
| Sound channel | good | good, wide | **excellent, long** | excellent, forward-biased |
| Night vision | poor | good | excellent | excellent |
| Memory / decay | **slow decay (long memory)** | fast, jumpy | medium | short outside focus |
| Attention focus | deliberate narrow *option* | always-wide, hard to focus | nose-led | **hard forward lock** |
| Detection certainty | high where it looks | high presence / low identity | high presence via scent, low live-position | high forward / near-zero rear |
| The characteristic *not-knowing* | "I know what, lost where" | "I know where, not what" | "I know who passed, not where they are now" | "everything ahead, nothing behind" |

---

## 5. Gameplay translation — biology, then *why it feels right*

The rule (D3): biology **inspires**; it does not command. Each mechanic below earns its place by the emotion it creates, not by fidelity.

- **Human — the planner who must look.** Medium cone, superb identification, slow memory decay. Biology: foveal acuity + spatial memory + persistence cognition. Gameplay: the human *recognizes* a far shape as prey and *remembers* where it went long after it's out of sight, but must physically turn attention to know what's behind. **Why it feels right:** it makes the human's fantasy cognitive — you win by reading and remembering, not by sensing more. The blind back creates deliberate looking; the long memory rewards planning. That is persistence hunting expressed as *knowledge*.
- **Deer — panoramic dread.** Near-360° motion field, terrible identification, fast decay. Biology: lateral eyes, motion-tuned retina, poor acuity. Gameplay: you *see* movement everywhere the instant it happens, but a motionless predator in plain sight stays an unresolved smudge — and your knowledge evaporates fast, so you must keep scanning. **Why it feels right:** this is the exact texture of prey anxiety — hyper-aware yet unable to *confirm*, safe only while everything holds still, punished for looking away. The stationary-predator gap turns the ambush into a genuine dilemma rather than a spot-check.
- **Wolf — reading the invisible.** Secondary vision, dominant scent that ignores line of sight, slow scent decay, wind-gated. Biology: olfaction as primary hunting sense, downwind approach, trail-age discrimination. Gameplay: large parts of the world stay visually unknown, but scent trails glow *through* terrain — fresh bright, old faint — and shift with the wind, so the wolf routinely knows *something came through here, recently, heading that way* without ever seeing it. **Why it feels right:** it makes the wolf a reader of the past, a fundamentally different cognition from vision — the player learns to trust the nose over the eyes, which is the single clearest "I think differently now" moment in the whole set.
- **Big cat — the cost of focus.** Narrow deep cone, excellent forward detail, near-zero rear awareness, short memory outside focus, hard target lock. Biology: forward binocular ambush vision, target fixation. Gameplay: everything you point at is crystalline and holds through brief occlusion; everything else barely exists, and a threat can close from behind unfelt. **Why it feels right:** power and blindness in one body — the fantasy of the apex ambusher is *not* omniscience, it's devastating commitment. The unfelt flank is the price, and it makes the cat tense to play despite being the strongest.

---

## 6. Technical architecture

A single generic framework; species are pure data.

```
Stimulus sources (movers, scent field, sound events, blood, tracks)
        │
        ▼
Per-agent Perception (a set of weighted Channels + a Wind read)
        │           each channel writes into ↓
        ▼
KnowledgeModel  ── two coupled stores ──►  Consumers
  • Entity ledger: per-entity rung (Absent/Detected/Classified/Tracked/Remembered) + confidence + timestamp
  • Spatial fog:   a coarse grid of "knownness" per cell, with decay
        │                                   ├── AI brain (reads as data)   ← same model (D7)
        │                                   └── Player renderer (draws the fog + known entities)
```

Design invariants:
- **One model, two consumers** (from `PERCEPTION_DESIGN.md` §5): the AI brains that Stage 2 needs read the *same* knowledge the player's screen renders. We are not building perception UI; we are building the perception system the player inhabits. Per-agent + server-computable keeps the multiplayer property free.
- **Configuration is data** (mirrors `UAnimalConfig`/D13): `USpeciesPerceptionProfile` grows into the channel-set config — visual/scent/sound params, memory half-life, attention focus, detection certainty. New species = new data asset, no code.
- **The fog is a coarse grid, not per-pixel.** A low-resolution 2D "knownness" field over the play space (tens of cm per cell is plenty at prototype scale) — cheap to update and decay, cheap to query for both AI and rendering. Static terrain is always known (Q18 ruling: static shown, living earned); the grid tracks *dynamic* knowledge and its freshness.
- **Never fabricate.** A cell/entity is only written when a channel *actually* delivers it. Decay lowers confidence toward Unknown; it never invents. Uncertainty is represented as low confidence / coarse rung, not as a guess.

### Rendering ideas (UE5, principles not commitments)
- **Fog as a screen-space or ground-projected field** driven by the knownness grid: unknown = obscured (a soft dark/desaturated veil that reads as *absence of information*, not nighttime), known = clear, remembered = a fading in-between. Candidate approaches: a post-process material sampling a render-target that holds the grid; or a large translucent decal/mesh over the ground reading the same texture. A compute/render-target update of the grid each frame is well within budget at this scale.
- **Known dynamic entities** render normally *only* at their rung: Tracked = the real actor; Remembered = a static, fading "ghost" at last-known transform (explicitly not updated); Detected = a crude ambiguous proxy (a disturbance, a shiver in grass) — never an icon.
- **Scent/tracks** as ground-projected, wind-advected trails visible *only* to nosed species — same render-target-driven decal idea, a second layer.
- **Wind** as a global parameter feeding the scent layer's advection and the sound radius — invisible itself, felt through what it reveals.
- **Debug view** (`firstlife.PerceptionDebug`, already in): draws the raw grid + channel geometry so *we* can see ground truth vs. the body's belief.

Reuse from what exists: `UScentFieldSubsystem` is already a stimulus source (keep, feed it into the model instead of drawing it); the slice-1 vision gating becomes the Visual channel writing into the ledger; the switcher/HUD stay as harness.

---

## 7. Advantages

- **A real pillar, not a filter.** The fog becomes a system players *play against* — the defining verb becomes *managing your own uncertainty*, which nothing else in the genre does per-species.
- **"Different game per species" made structural.** H8/H17 stop being aspiration: the same map is a different problem because the channel set is different. This is the cheapest possible proof of the whole vision.
- **Science as mechanic, honestly.** Each species' cognition is grounded (§2) and each liberty is flagged (D3) — the scientific identity pays off as *feel*, not decoration.
- **Emergent stories from ignorance.** The best survival stories are about what you *didn't* know (the flank you never saw, the trail you trusted). The fog manufactures those (H4/H16).
- **One system serves AI and player** (D7), and stays server-authoritative for the multiplayer property we protect for free.

## 8. Risks

- **Frustration vs. immersion — the central risk.** Hidden information can read as unfair or as a bug, especially at greybox fidelity where the "Detected" proxy is crude (same false-negative danger as H3, H15). Mitigation: start generous (rungs err toward *more* knowable), enforce the no-fabrication rule so the player learns the fog is honest, and tune outward.
- **Legibility of absence.** The fog must read as *"I don't know what's there,"* not as darkness or a graphical glitch. This is a hard rendering problem and the make-or-break of the whole idea.
- **Scent/wind complexity creep.** The wolf's channel is the richest and most tempting to over-build. Prototype scope must cap it (fixed-ish wind, breadcrumb trails) until the *reading* is proven fun.
- **Cost & tuning surface.** A grid + per-agent ledger is real engineering, and every channel multiplies tuning. Prototype scale (≤ a dozen agents, coarse grid) is cheap; the design must never assume per-blade or per-pixel sensing.
- **Camera can leak information** (D14): the wide isometric view must not hand the player knowledge the body lacks — the fog has to sit convincingly *over* a vantage that can technically see everything.
- **RTS drift** (Q20): a top-down fog-of-war visually resembles strategy games. Embodiment (direct control, body-anchored feedback, one-creature framing) must actively fight that read.

## 9. Playtest hypotheses

Each is falsifiable and testable on the greybox map with the four species.

- **PH1 — Distinct cognition.** Testers describe switching species as *"I have to think differently"* (channel-led), not *"the screen looks different."* → the headline success condition.
- **PH2 — Human plans and remembers.** The human player uses memory of last-known positions and deliberately turns to check blind areas; narrates intent about where things *went*.
- **PH3 — Deer dread.** Deer players stay jumpy and reactive, walk past a motionless predator, and alarm the instant it moves. They describe feeling wide-aware but unable to confirm.
- **PH4 — Wolf trusts the nose.** Wolf players find an animal they never see, via scent, and narrate the past ("something came through here, heading that way, not long ago"). Wind direction visibly changes their route.
- **PH5 — Cat's blind power.** Big-cat players feel dominant while locked forward and get flanked unseen at least once, describing it as a fair trade, not a cheat.
- **PH6 — Honesty trust.** Across sessions, players come to *act on* uncertain information (investigate a Detected smudge, follow a faint trail) — evidence they've learned the fog never lies.
- **PH7 — Not-unfair.** The hiding is described as tense/immersive, not as broken. Any "that felt unfair" moment is logged with exactly when and why (this is the most valuable output if the answer is negative).

---

## 10. Smallest agreed build order (after sign-off — not started)

1. **Spatial fog grid + Visual channel.** Coarse knownness grid; the Visual channel (slice-1 gating, generalized) writes and refreshes it; unknown cells veil. Human vs. big cat already feel different with nothing else.
2. **Memory decay + Remembered ghosts.** Knownness fades over a species half-life; last-known entity ghosts. The human's long memory vs. the cat's short one appears here.
3. **Motion channel + Detected proxies.** Wide motion entry with crude ambiguous proxies. Deer becomes real; the stationary-predator dilemma exists.
4. **Scent channel + wind.** Reroute `UScentFieldSubsystem` into the model as a nosed-species channel; trails visible through the fog, wind-gated. Wolf becomes real.
5. **Attention postures** (one per species, from `PERCEPTION_DESIGN.md` §4) only after 1–4 read as fun.

Each step is independently playable and testable against §9.

---

## 11. Open questions for sign-off

1. **Fog rendering commitment:** post-process material vs. ground decal driven by a render-target — cheap prototype spike to decide, or pick one now?
2. **Generosity dial:** do we start deliberately *too knowable* (safer for PH7) and tighten, or start honest-to-biology and loosen? (I recommend start-generous.)
3. **Scope of slice 1:** pure Visual + fog grid only (my recommendation — proves the scariest risk, legibility of absence, with least code), or include Memory decay in the first playable?
4. **Wind in prototype:** fixed vector (cheapest) vs. slowly rotating — affects how early the wolf's "wind changes what I know" reads.
