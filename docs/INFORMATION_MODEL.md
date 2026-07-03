# The Universal Information Model — Umwelt Primitives

> Status: **theory / architecture proposal, awaiting agreement. No code.**
> Relationship to prior docs: this is the substrate *beneath* `SPECIES_COGNITION.md`. That doc named four objects (Stimulus Field → Channels → Umwelt → Consumers) and framed them per species. **This doc reduces those four objects to their irreducible primitives — the smallest universal information model — and shows that every species is a choice of *constants*, not a system of its own.** If `SPECIES_COGNITION.md` is the biology and the pillar, this is the physics.
> Method note (as instructed): this document does **not** reason from humans, wolves, or deer. It reasons from *information* — what a stimulus is, how it ages, how uncertainty moves — and then derives the species at the very end (§9) as a corollary. If a species had to be special-cased, the model would have failed.

---

## 0. The question, and the answer in one paragraph

**Question:** if Umwelt is the foundation, what is the smallest universal information model from which every species' subjective reality emerges naturally?

**Answer:** **one noun and three verbs.** The noun is the **Trace** — a localized, timestamped, decaying *claim* about the world, carrying a bundle of features each with its own fidelity. The three verbs are **Age** (evolve a Trace forward in time — the one operator that unifies decay, diffusion, drift, *and* prediction), **Sample** (copy a degraded Trace from one store into another through an aperture — the one operator that unifies both emission and sensing), and **Fuse** (integrate an incoming Trace into a store of held Traces). A world holds Traces (a footprint, a scent, a sound, a photon-pattern). A mind holds Traces too (we call those *beliefs*, but they are structurally identical). Perception is Sampling from the world's store into the mind's store; memory is a belief no Sample is refreshing; prediction is Age carrying a belief along its own motion estimate; uncertainty is the spread and strength that Age can only ever worsen and Sample can only ever lose. Species are not built. They are the choice of *which apertures exist* and *how fast their held Traces age*. That is the entire model.

---

## 1. The reduction: from four objects to one primitive

`SPECIES_COGNITION.md` gave four objects. Watch them collapse:

- The **Stimulus Field** is a store of Traces (in the world).
- The **Umwelt / belief store** is a store of Traces (in an agent). *Same structure.*
- A **Channel** is a Sample-through-an-aperture operation.
- The **Consumers** (AI, renderer) just read a Trace store.

So the four objects are really **two stores of the same primitive**, connected by **operators**. The Trace is the atom. Everything else is a store of Traces or an operation on Traces. This is the reduction the rest of the doc formalizes.

The claim to keep testing as you read: **there is nothing in any species' subjective reality that is not either a Trace, a store of Traces, or one of three operations on Traces.** If you find a counterexample, the model is incomplete — that is the falsification test.

---

## 2. The single state primitive — the Trace

A **Trace** is a decaying, localized claim. Its anatomy is four fields:

```
Trace {
  where     : a spatial estimate — NOT a point, a distribution
              { center, spread, heading? }        // blob + optional direction
  what      : a feature bundle, each with its own fidelity
              [ (feature, value, fidelity 0..1), ... ]
  when      : birth timestamp                      // the moment the claim was created
  strength  : 0..1                                 // detectability / overall confidence
}
```

Plus two pieces of *governing data* that are not the claim itself but ride with it:

```
  transport : a vector or null                     // how `where.center` moves as it ages
  decay     : the aging constants                  // rates for strength, spread, per-feature fidelity
```

Four points make this primitive load-bearing:

**(a) `where` is a distribution, not a coordinate.** A Trace never says "at (x, y)." It says "somewhere around here, this tightly." A point is just a distribution with zero spread — the special case of perfect certainty, which almost nothing in this world enjoys. Representing space as a blob is what lets uncertainty *be a first-class quantity* instead of a flag.

**(b) `what` is a bundle of features with *independent* fidelities.** This is the subtle, essential field. A Trace does not degrade uniformly. A scent can keep "kind: canine" sharp long after "where" has diffused into a wide vague lobe. A distant shape can keep "where" sharp while "kind" stays unknown. Because each feature carries its own fidelity, and Age erodes them at *different* rates, the model produces the exact textures the biology demands — "I know *where* but not *what*" (deer) and "I know *what* and *when* but not *where-now*" (wolf) — from one structure. No per-species code; just different feature-decay rates.

**(c) `when` makes time explicit.** Every Trace knows its own age (`now − when`). This is the field that makes `SPECIES_COGNITION.md`'s "temporal depth of perception" mechanical: a sense is defined partly by *which ages of Trace it can read* (§4). Vision reads only age≈0 Traces; scent reads age = minutes-to-hours Traces.

**(d) `strength` is the honesty valve.** It runs to zero and the Trace vanishes. Nothing lingers forever; the world's memory and the mind's memory are both finite because strength decays to nothing. When strength hits zero, the claim is *gone* — not "unknown-but-suspected," gone. Absence is absence.

### The unification that earns the whole model

**A world-trace and a belief are the same primitive.** A footprint pressed in mud is a Trace the *world* holds: `where` = the print's location (tight), `what` = [(kind, deer, 0.8), (heading, NE, 0.6)], `when` = when it was made, `strength` = fading as it weathers. A memory of a deer is a Trace a *mind* holds: `where` = last-known blob (spreading), `what` = [(kind, deer, 0.9)], `when` = when last seen, `strength` = fading as attention moves on. **Identical fields. Identical aging law. Different owner and different decay constants.** The world remembers by physics; the mind remembers by neurology; the data structure does not care which.

This is not a cute analogy — it is the reason the model is small. We do not build a scent system *and* a memory system *and* a tracking system. We build one Trace and one place-it-can-live abstraction, and footprints, scents, sounds, glimpses, and memories are all instances.

---

## 3. The evolution operator — `Age(trace, dt)`

There is exactly one operator that moves a Trace through time, and it does four things that are usually thought of as four separate systems:

```
Age(trace, dt):
    trace.strength    *= decay_strength(dt)          # (1) DECAY   — detectability fades
    trace.where.spread += diffuse(dt)                # (2) DIFFUSION — location gets vaguer
    trace.where.center += trace.transport * dt       # (3) TRANSPORT — location moves
    for each feature f:
        f.fidelity    *= decay_feature[f](dt)        # (4) EROSION — features drop out one by one
```

1. **Decay** — strength falls toward zero. The Trace becomes harder to detect and eventually disappears. (A scent evaporates; a memory fades.)
2. **Diffusion** — the spatial spread grows. The claim gets *less specific about where* the older it is. (A scent lobe widens; a remembered position becomes "somewhere over there.")
3. **Transport** — the center *moves* along the `transport` vector. For a **world-trace**, transport is the **wind** (scent drifts downwind as it ages). For a **belief**, transport is the held **velocity estimate** (a thing keeps going where it was heading). *This is the same line of code.*
4. **Erosion** — each feature's fidelity decays at its own rate, so features drop out in sequence: a memory goes from "wolf" → "predator-ish" → "something" → gone. Coarsening is just erosion applied to the `what` bundle.

**How stimuli age and decay** (your first two sub-questions) is entirely answered by Age. There is no separate decay system.

**How prediction emerges** (your hardest sub-question) is answered by term (3). Prediction is *not a new operator.* Prediction is Age's transport term applied to a belief that happens to carry a velocity feature. Run the belief forward: its center slides along its last-known heading; simultaneously its spread grows (term 2), so the forecast is honestly a *widening cone*, not a confident point — you predict *roughly* where it went, less surely the further ahead you look. And here is the constraint that makes species emerge for free: **an agent whose beliefs do not retain a velocity feature has `transport = null` and therefore cannot predict at all.** The deer does not "have prediction disabled." Its Umwelt simply never stored a heading, so Age has nothing to transport. Prediction is present or absent as a *consequence of what the belief holds*, never as a feature toggle.

Note the poetry that is also good engineering: **a scent drifting on the wind and a hunter's mind projecting where the prey went are the same operation** — transport of a decaying, spreading Trace along a vector. One is wind; one is velocity. The universe of this game runs one function for both.

---

## 4. The transfer operator — `Sample(aperture, source_store) → percepts`

A sense is an **aperture**: a filter that copies Traces out of one store into a percept stream, degrading them on the way. Its anatomy:

```
Aperture {
  geometry     : the region of space it reads     // cone, wide arc, wind-shaped lobe, radius
  modality     : which Traces it can read at all   // light / sound / chemical / substrate
  age_window   : which ages of Trace it can read   // [0,0] vision · [0,~5s] sound · [min,hours] scent
  threshold    : minimum strength to register      // below this, the Trace is not sensed
  feature_pass : which features it copies           // vision→{where,kind}; motion→{heading} only
  acuity       : how much fidelity survives copying // the sensor's own added blur
}
```

Sampling intersects the aperture's geometry with the source store, and for every Trace that clears `threshold`, is within `age_window`, and matches `modality`, it emits a **percept** — a fresh Trace copy carrying only the features `feature_pass` allows, each attenuated by both the source Trace's current fidelity *and* the aperture's `acuity`.

Two unifications here:

**(a) Emission and sensing are the same operation.** An event writing scent into the world is a Sample through a "broadcast aperture" (the event is the source, the world store is the destination). An eye reading the world is a Sample through an "intake aperture" (the world is the source, the mind is the destination). Both are "copy degraded Traces from store A to store B through an aperture." We do not need an emit system and a sense system; we need Sample with the source and destination swapped.

**(b) The four biological "channels" are one aperture with different constants.** Vision is an aperture with `age_window=[0,0]`, straight-line geometry (occlusion), high `feature_pass`={where, kind}, high `acuity`. Scent is an aperture with `age_window=[minutes,hours]`, wind-advected lobe geometry, `feature_pass`={kind, heading, age}, low positional `acuity`. Motion-detection is an aperture that passes only `{heading}` and only from Traces whose strength is *changing*. Hearing is a radius aperture, `age_window`≈seconds, `feature_pass`={region, kind-ish}. **There is no "Channel" class hierarchy. There is one Aperture struct and a table of numbers.**

**How senses sample** (your fifth sub-question) is exactly this operator. And "each species reads a different depth of time" is now literally the `age_window` field: it is the mechanical home of the temporal-depth idea.

---

## 5. The integration operator — `Fuse(store, percept)`

When a percept arrives, it must be integrated into the mind's store of held beliefs. `Fuse` is the only operator that can *increase* certainty, and only by consuming a real Sample of the world:

```
Fuse(store, percept):
    match = find_belief_consistent_with(store, percept)   # same-ish where + what?
    if match:
        reinforce(match, percept)      # strength↑, spread↓ (shrinks toward the new evidence),
                                       # fidelity↑ where they agree, when := now
    elif contradicts(match):
        compete(match, percept)        # stronger/fresher evidence wins; loser weakens
    else:
        instantiate(store, percept)    # a new belief is born
```

`Fuse` is the "update" that pulls a belief back toward reality when the senses re-contact it. Between Fuses, the belief is alone with `Age`, drifting and fading. So the life of a belief is a loop: **Fuse sharpens it (a real observation), Age blurs it (time without observation), Fuse sharpens it again** — the tighter the loop, the more accurate the belief; break the loop (lose sight) and the belief decays into vague, drifting memory.

**How memory transforms** (your sixth sub-question) is now fully specified: memory is a belief in the *Age-only* phase of that loop. Its transforms are exactly Age's four terms — it **fades** (decay), **vagues out** in position (diffusion), **drifts** along its last heading if it kept one (transport → this is the human's projected ghost), and **coarsens** feature by feature (erosion → "wolf" becomes "something"). Memory is not a separate faculty; it is the absence of Fuse.

---

## 6. Recursive Bayesian estimation — the rigorous spine (and what we will *not* build)

This model is not hand-waving. It is a deliberately lossy, gamified **recursive Bayesian filter** — the same mathematics robots use to build a world model from noisy sensors:

- A belief's `{where, strength}` is a **state estimate with a covariance**.
- `Age` is the filter's **predict** step: propagate the state along the motion model, and *grow the covariance* (uncertainty always increases when you're not looking).
- `Fuse` is the filter's **update** step: a measurement *reduces* covariance, but only in proportion to the measurement's own reliability.
- The Kalman/particle-filter literature is the credibility anchor: recursive estimation from partial, noisy, aging observations is a solved, well-understood problem, and "predict grows uncertainty, update shrinks it" is its core.

**But we implement the *intuition*, not the matrices.** No Kalman gain, no covariance algebra. `where.spread` is a scalar radius; `Age` adds to it; `Fuse` subtracts from it. That is enough to produce the entire felt experience and it runs in a few floating-point ops per Trace. The Bayesian framing exists to prove the model is *sound* and to borrow its guarantees — not to license implementing a robotics stack in a solo-dev game. `[design: rigor as inspiration, per CLAUDE.md pillar 5]`

---

## 7. How uncertainty propagates — and the honesty invariant

Your fourth sub-question deserves its own section because the *answer is a constraint on the operators*, and that constraint is the game's trust contract expressed as mathematics.

**Uncertainty has exactly four sources, and each is one of our primitives:**
1. **World decay** — a Trace ages in the world before anyone senses it (Age on a world-trace: the scent was already vague when you found it).
2. **Aperture loss** — Sampling adds the sensor's own blur (`acuity` < 1: even a fresh scent is imprecisely placed by a weak nose).
3. **Belief decay** — the held belief ages between observations (Age on a belief: memory blurs).
4. **Fusion conflict** — contradictory percepts leave a belief less sure than either alone (Fuse's compete branch).

**The honesty invariant (the trust contract, formalized):**

> **No operator may raise a feature's fidelity, raise a Trace's strength, or shrink its spatial spread — *except* `Fuse` incorporating a genuinely fresh `Sample` of the real world.**

Consequences, which are the whole point:
- `Age` is **monotone in uncertainty**: it can only fade, spread, drift, and coarsen. It never sharpens. Time alone never makes you *more* sure.
- `Sample` is **strictly lossy**: a percept is never more certain than the world-trace it copied. Sensing never adds information that wasn't there.
- Therefore **the only source of certainty in the entire universe is contact with ground truth**, through `Fuse`. Certainty is *earned* by observation and *decays* by everything else.
- **Fabrication is structurally impossible.** There is no operator whose output invents a feature value or a false position. The worst the system can do is be *vague* (low fidelity, wide spread, low strength) — never *wrong-with-confidence*. A "something moved, north-east" belief means something did move north-east; it simply declines to say what. This is `SPECIES_COGNITION.md`'s "never fabricate" rule, now enforced not by discipline but by the *type of the operators*.

That last point is the deepest safety property of the design: the trust contract that makes players willing to act on partial information is not a rule we must remember to follow — it is a shape the operators cannot violate.

---

## 8. The complete minimal specification

The whole universal model, in one place:

```
STATE
  Trace = { where:{center,spread,heading?}, what:[(feat,val,fidelity)], when, strength,
            transport?, decay }
  Store = a collection of Traces           # the world has one; each agent has one

OPERATORS
  Age(trace, dt)            # decay + diffusion + transport + erosion   (§3)
  Sample(aperture, store)   # emission (event→world) OR sensing (world→mind), lossy   (§4)
  Fuse(store, percept)      # integrate; the only certainty-increasing op   (§5)

SENSE
  Aperture = { geometry, modality, age_window, threshold, feature_pass, acuity }

AGENT
  Species = { apertures:[Aperture], belief_decay_constants }
            # THAT IS ALL a species is.
```

Everything in `SPECIES_COGNITION.md`'s architecture is here: the Stimulus Field is the world Store; the Umwelt is an agent Store; the Channels are Apertures + Sample; the Consumers read a Store. Nine lines of concept.

---

## 9. The species, derived (the corollary — done last, on purpose)

If the model is right, no species needed to be designed — each is a row of constants. Here they fall out:

| | **apertures it has** | **belief-decay (memory)** | **transport in beliefs?** | **emergent subjective reality** |
|---|---|---|---|---|
| **Deer** | wide motion-aperture (age_window=[0,0], feature_pass={heading}, huge geometry, low acuity); weak vision | fast, jumpy | **null** — beliefs keep no velocity | near-panoramic *change* at time≈0; can't say *what*, can't project *forward* → lives in unresolvable present. "What moved?" |
| **Wolf** | chemical aperture (age_window=[min,hours], feature_pass={kind,heading,age}, wind-advected lobe); mediocre vision | slow for scent-sourced beliefs | forward, weak | reads Traces *hours old* off the ground → reconstructs recent past. "Who passed here, and when?" |
| **Big cat** | narrow deep vision-aperture (age_window=[0,0], high acuity, forward geometry only); almost nothing else | very short outside a locked belief | forward, only for the locked target | a sharp deep *forward present*, nothing behind or before. "Can I close the gap before it runs?" |
| **Human** | medium vision (high `kind` acuity, trichromatic feature_pass), plus a track-reading aperture on substrate Traces | **slow** — the superpower | **forward, strong** — beliefs retain and project velocity | senses least, but Fuse+Age let it hold and *run beliefs forward* → reads past *and* future. "Where is it going next?" |

Not one row required a new operator, a new primitive, or a special case. The "different nervous system" is four different rows of the same table. That is the proof the model is minimal *and* sufficient: subjective reality is a function of `(apertures, decay constants)` and nothing else.

---

## 10. What the primitives deliberately exclude (scope guard)

The model is small because these are **not** primitives — they are *consumers* of beliefs, built later, on top, if at all:

- **Reasoning / classification** — not an operator. "What is it" is a *feature with a fidelity*; a thing is "classified" when its `kind` fidelity is high. No classifier, no logic tree.
- **Emotion / fear** — not modelled. An instinct is a *trigger*: a rule that fires an action when a belief matches a pattern (motion-onset in periphery → flee). A short list of triggers reads the Store; it is not part of the Store.
- **Learning** — no operator changes the decay constants or apertures over time. Deferred entirely.
- **Pathfinding / decision-making** — consumes the Store; lives in the AI brain, not the substrate.
- **A point-position anywhere** — there are no certain coordinates in the model except as the degenerate zero-spread case; the world is made of blobs.

If a future feature cannot be expressed as a Trace, a Store, or one of the three operators, that is the signal to stop and ask whether it belongs in the substrate at all — the answer will almost always be that it belongs in a *consumer*.

---

## 11. Open questions for sign-off

1. **Spatial representation of `spread`:** scalar radius (cheapest, isotropic) vs. a 2×2 covariance (directional uncertainty — a scent lobe is longer along the wind than across it). Recommendation: **start scalar**, add anisotropy only if the wolf's directional scent doesn't read without it.
2. **Feature fidelity granularity:** is a flat `[(feature, value, fidelity)]` list enough, or do features need dependencies (you can't know `heading` without some `where`)? Recommendation: **flat list first**; add dependencies only if a species produces an incoherent belief (heading with no position).
3. **Store cost at scale:** world-Store growth is the real budget risk (every mover emits Traces forever until decay). Needs a hard cap + spatial hashing before the Stimulus Field slice. Prototype scale (≤ a dozen agents, coarse grid) is safe; the design must never assume per-blade emission.
4. **Is `Fuse`'s compete branch needed in slice 1?** Probably not — reinforce + instantiate covers vision-only. Defer conflict resolution until two channels can disagree (scent vs. sight).
5. **Does the world-Store and the debug renderer give *us* the omniscient view we need to tune, without ever exposing it to the player?** (It should — the world Store *is* ground truth — but the guarantee that no player-facing renderer reads the world Store directly must be an architectural invariant, not a convention.)
