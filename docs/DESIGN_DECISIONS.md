# Design Decisions

> A log of the high-level decisions already made, each with *why* and *what tradeoff it accepts*.
> This is a decision record, not a rulebook. Every entry is a choice with a cost — naming the cost is the point.
> When a decision is revisited or reversed, do not delete it. Add a dated note. The history is what keeps us honest.

## How to read this

Each decision states the call, the reasoning, the tradeoff we knowingly accept, and its current status. A decision with no stated tradeoff is not finished thinking — every real decision costs something.

Format:
- **Decision** — what we chose.
- **Why** — the reasoning.
- **Tradeoff accepted** — what we gave up, on purpose.
- **Status** — Firm (load-bearing, changing it changes the game) / Working (holds until evidence says otherwise) / Provisional (a default we expect to revisit).

---

### D1 — The ecosystem is the product; species are perspectives into it

**Why:** It is the one idea that makes the project both distinctive and buildable by a small team. We invest in one living world and open windows into it, rather than building many games.
**Tradeoff accepted:** Enormous up-front investment in simulation and AI before there is a "complete" game to show. The payoff is deferred and the early months look unimpressive to outsiders.
**Status:** Firm. This is the identity. If this changes, it is a different project.

### D2 — Gameplay before realism

**Why:** Real animal life is often idle, repetitive, and brutal. Fidelity is not fun by default. When the two conflict, the experience wins.
**Tradeoff accepted:** We will knowingly misrepresent nature at times, and purists may object. We accept selective inaccuracy as the price of engagement.
**Status:** Firm. (This is in direct, deliberate tension with D3 — see the tensions section.)

### D3 — Scientific accuracy inspires mechanics; it does not command them

**Why:** Biology is our richest source of *ideas* — real senses, real predator-prey dynamics, real herd behavior are better than anything we'd invent. But it is a well to draw from, not a cage.
**Tradeoff accepted:** We take on the ongoing discipline of deciding, case by case, when to follow biology and when to break it — and documenting which. That is real, recurring work.
**Status:** Firm.

### D4 — AI and simulation matter more than graphics

**Why:** Believable behavior is the product; visual fidelity is not. Emergent stories come from animals that act convincingly, and no amount of polygons compensates for animals that act stupidly. It is also the honest allocation for a solo dev: art can be bought, believable behavior cannot.
**Tradeoff accepted:** The game will, especially early, look less impressive than it plays. It will not win on screenshots. We are betting that "feels alive" beats "looks amazing" — a bet the market does not always reward.
**Status:** Firm. *Caveat (see tensions): animals communicate state largely through animation and visual readability, so "AI over graphics" has a floor — presentation cannot be neglected entirely.*

### D5 — Prototype before production

**Why:** The core assumption (that inhabiting an animal in a living ecosystem is engaging through simple systems) is unproven. Building production content on an unvalidated core is the most expensive possible mistake.
**Tradeoff accepted:** Slower path to anything that looks like "a game." We spend real time on grey-box prototypes that may be thrown away. Discipline now to avoid catastrophe later.
**Status:** Firm.

### D6 — Small systems creating emergent gameplay, over big authored features

**Why:** Stories that emerge from simple interacting systems are cheaper to build, more replayable, and more surprising than authored content. It is also the only content model a solo dev can sustain — you cannot hand-author a world alone.
**Tradeoff accepted:** Less control over the moment-to-moment experience. Emergent systems can produce dull or broken situations, and tuning them is subtle, open-ended work. We trade authored polish for generative depth.
**Status:** Firm.

### D7 — Possession architecture: the player is a possessed AI agent

**Why:** Making the player just another animal-agent (whose decisions are handed to a human) is what turns "inhabit any species" and "multiplayer" into future features rather than rewrites. It is the single technical decision that keeps the whole long-term vision reachable.
**Tradeoff accepted:** Some up-front architectural discipline in the prototype for a payoff that only fully materializes later. We build the general case (any agent playable) while exposing the specific case (one wolf).
**Status:** Firm.

### D8 — Multiplayer is a consequence of the architecture, not a roadmap milestone

**Why:** Because the player is a possessed agent, N players in one simulation is something the design already permits. Chasing netcode before the core is proven would be spending our scarcest resource on scale we haven't earned.
**Tradeoff accepted:** We deliberately leave a headline feature (multiplayer) unbuilt and unpromised. If the market expects it early, we disappoint that expectation on purpose.
**Status:** Working. Revisit only after the single-player core is validated fun.

### D9 — One playable species first; more are earned, not planned

**Why:** Proving the core with the cheapest, most legible animal (the wolf) de-risks everything. Every additional species is a post-validation feature, gated on the previous one being genuinely fun.
**Tradeoff accepted:** The most exciting part of the pitch ("be any animal") is deferred the longest. We resist our own most marketable idea to protect scope.
**Status:** Firm.

### D10 — Solo development, long haul, buy what can be bought

**Why:** Matches reality. One developer over a long horizon must spend scarce time only on what cannot be outsourced (systems, AI, design) and buy the rest (art, audio, animation assets).
**Tradeoff accepted:** A bought-asset look means less visual uniqueness and some integration friction. We accept a less bespoke surface to keep the irreplaceable work funded with time.
**Status:** Firm (for the foreseeable phase).

### D11 — The world is indifferent to the player

**Why:** The indifferent living world is the emotional foundation (`GAME_DNA.md`). Systems run whether or not the player acts; nothing rubber-bands to keep the player central or safe.
**Tradeoff accepted:** Indifference can read as aimlessness or unfairness, especially for new players. We accept a harder onboarding and a narrower audience in exchange for the core feeling. (This is the sharpest unresolved tension in the whole design — see `DESIGN_QUESTIONS.md`.)
**Status:** Firm as principle; the *degree* is Provisional.

### D12 — Naming deferred until the prototype tells us what the game is

**Why:** A name chosen now anchors us to a guess. The prototype will reveal what the game actually feels like, and the name should follow that truth.
**Tradeoff accepted:** No brand identity, no marketing hook, harder to talk about the project externally in the meantime.
**Status:** Working.

---

## Acknowledged tensions between decisions

Honest decision records surface where their own decisions fight each other. These are not problems to hide; they are the design's real pressure points.

- **D2 (gameplay before realism) vs. D3 (accuracy inspires).** These are the same coin, and the friction is permanent. Every biological detail is a case-by-case ruling: inspiring or constraining? The decision framework does not resolve individual cases — it just guarantees we *decide on purpose* and write down which way we went.
- **D4 (AI over graphics) vs. the readability floor.** Animals broadcast intent and emotion through motion and visual cues. You cannot fully separate "believable AI" from "legible presentation." AI-over-graphics is a priority, not a license to neglect the animation and readability that make the AI *legible*.
- **D6 (emergent systems) vs. D5 / player legibility.** Emergent systems resist authored guarantees. We cannot promise any specific moment will happen, which complicates onboarding, tutorials, and marketing beats. We trade predictability for depth and must find non-authored ways to make the game legible.
- **D11 (indifferent world) vs. reaching an audience.** The single biggest risk in the design. An indifferent world is the soul of the game and potentially the wall between it and a wide audience. We have chosen the soul. Whether we can make it *approachable* without betraying it is the central open question of the project.
- **D8/D9 (defer multiplayer and extra species) vs. commercial expectations.** Our scope discipline defers exactly the features most likely to sell the game. We are betting that a deep, distinctive core earns the right to those features later — rather than that a broad, shallow feature list wins now.

When you make a new significant decision, add it here with its tradeoff. When two decisions collide in practice, add the collision to this list. The value of this document is proportional to how honestly it records what our choices cost.
