# ARCHITECTURE DECISIONS (Engineering ADRs) — The First Life

> Technical decision record. Complements `docs/DESIGN_DECISIONS.md` (which owns *game/design* decisions
> D1–D14); this file owns *engineering* decisions that constrain how agents implement those.
> Format per entry: **Decision · Why · Consequence/Tradeoff · Status** (Firm / Working / Proposed).
> Proposed ADRs are architecture the Chief Architect intends but that a gate (see `TASK_GRAPH.md`) has
> not yet unlocked. Maintained by the Chief Architect.

---

### ADR-E1 — The Umwelt four-layer is the canonical perception architecture

**Decision:** Adopt `docs/SPECIES_COGNITION.md` §5 as the canonical technical spec: `World → ① Stimulus
Field → ② Channels → ③ Umwelt (per-agent belief store) → ④ Consumers`. The other three perception docs
are superseded-in-framing.
**Why:** Four docs converged on this after three reframes (overlays → knowledge-gating → fog → belief
store). The consistency audit confirmed no architectural conflict, only terminology drift.
**Consequence:** Terms unify — *Umwelt/belief store* (not "KnowledgeModel"); channels = {Vision, Motion,
Scent, Sound}; *Memory is a rung, not a channel*; *Wind is a Stimulus-Field modifier, not a channel*; two
independent decays (field fade + belief-confidence fade).
**Status:** **Proposed — blocked on GATE-B sign-off.** No code until the owner blesses it.

### ADR-E2 — The belief store is a standalone per-agent object, not player UI plumbing

**Decision:** The Umwelt is its own object owned per-agent (on `AAnimalCharacter` or its perception
component), holding an entity ledger (rungs + confidence + age) and a coarse spatial fog grid. It is **not**
inlined into the player renderer.
**Why:** Today the embryonic belief store (`HiddenTargets`/`SecondsSinceSeen`) lives inside
`USpeciesPerceptionComponent` and runs only for the player body. That coupling blocks the entire payoff.
**Consequence:** A refactor (node U1) extracts it; the Vision channel writes into it; the renderer reads
from it. Mostly additive — the vision-gate half already exists.
**Status:** Proposed (U1), blocked on GATE-B.

### ADR-E3 — One model, two consumers (D7 extended to the senses)

**Decision:** The AI brain and the player renderer consume the *same* per-agent Umwelt. AI acts only on its
beliefs (e.g. pursues last-known position), never on world-truth.
**Why:** This is the possession bet (D7) applied to perception. It makes animals believable, makes the hunt
fair (predator & prey reason from the same partial data), and keeps everything per-agent + server-computable
— preserving the "multiplayer for free" property (D8).
**Consequence:** AI cannot short-circuit to ground truth; a debug renderer draws truth for *us* only.
**Status:** Proposed (node U4); the highest-value payoff of Track U.

### ADR-E4 — A species is data assets on one pawn class; never a subclass

**Decision:** Every animal is one `AAnimalCharacter` configured by `UAnimalConfig` (locomotion/energy) +
`USpeciesPerceptionProfile` (senses). Adding a species = new data assets, no new class, no recompile.
**Why:** D13; it is what makes "inhabit any species" a feature not a rewrite (H7).
**Consequence:** No `if (species == …)` branching in behaviour code. The *only* permitted species branch
today is `EPerceptionOverlayStyle` for debug colours — keep even that minimal.
**Status:** **Firm — already realised in code and verified by the code map.**

### ADR-E5 — Systems live in components; the character owns wiring only

**Decision:** `AAnimalCharacter` owns no gameplay logic — it wires components and the camera. Systems
(`ULocomotionComponent`, `UStaminaComponent`, `USpeciesPerceptionComponent`, future Umwelt/AI) own their
own behaviour and read config.
**Why:** Cohesion, testability, and it keeps the pawn controller-agnostic (the possession seam).
**Consequence:** `AAnimalCharacter` is the one high-merge-risk file (everything hangs components on it).
Edits to it are serialised through Integration (see ADR-E7).
**Status:** Firm.

### ADR-E6 — The Stimulus Field generalises the scent placeholder

**Decision:** `UScentFieldSubsystem` (flat FIFO point array today) is the seed of the species-agnostic
Stimulus Field: append-and-decay records (motion pulses, sound, scent, tracks, blood) with position,
features, timestamp, decay rate; wind reshapes it.
**Why:** It is layer ① of ADR-E1 and the mechanical basis of the signature "the world remembers" mechanic.
**Consequence:** Grid-vs-array and diffusion model are deferred; **not touched until scent enters** (build
step 5 / node U6). Keep the placeholder running and player-invisible until then.
**Status:** Proposed (node S1), design-gated with GATE-B.

### ADR-E7 — Integration policy: main stays green, ownership is disjoint

**Decision:** (1) Every merged change must compile (`Result: Succeeded`, FirstLifeEditor Mac Development).
(2) Ownership is disjoint per track (see `TASK_GRAPH.md` §5). (3) Edits to the shared seam
`AAnimalCharacter` are serialised through the Chief Architect — no two agents add components to it
concurrently. (4) Agents return diffs/reports; the Chief Architect reviews for drift, duplication, scope
creep, and DNA violations before merge.
**Why:** Solo-dev + parallel agents = merge risk concentrated on a few files. Discipline here is what makes
parallelism safe.
**Consequence:** Some serialisation cost on the hot file; worth it.
**Status:** Firm (governance).

### ADR-E8 — No premature netcode, no premature Mass Entity

**Decision:** Ordinary Actors, single-player, ≤ a dozen agents. No replication code beyond leaving the
possession-authority TODO. No ECS until herd scale is a *measured* bottleneck.
**Why:** VISION.md: multiplayer is a consequence of the architecture (per-agent, server-authoritable by
construction), not a milestone; Mass Entity is a later optimisation. Building either now is scope creep.
**Consequence:** The possession toggle is not authority-safe (documented); acceptable at this stage.
**Status:** Firm.

### ADR-E9 — Perception renders diegetically; no new HUD; canvas HUD stays

**Decision:** Percepts render in-world (proxies, ghosts, trails, the fog veil), never as icons/meters. The
only meters are the player's own body state (stamina). The canvas HUD (`AFirstLifeHUD`) stays; no UMG until
a real UI need is proven.
**Why:** SPECIES_COGNITION §7 scope wall; embodiment over UI (Q20/RTS-drift discipline).
**Consequence:** The fog renderer (U2) needs a post-process material or ground decal (binary asset / spike),
not a HUD widget. That is GATE-B blocker #3.
**Status:** Working.

---

## Log of engineering decisions taken this session

- **2026-07-03** — Removed the `AndroidFileServer` config block (incl. a checked-in `SecurityToken`) from
  `Config/DefaultEngine.ini`. Desktop-only project; plugin inactive; template leftover. Config-only, no
  behaviour change. (Hygiene / secret-in-VCS removal.)
