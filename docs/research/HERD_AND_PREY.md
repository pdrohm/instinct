# Research Brief — Herd & Prey for Slice 2 ("The Living Herd")

> Research Engineer brief feeding the **Slice 2** sprint (validates **H5**: an
> autonomous, believable herd makes the world feel alive). Companion to
> `PROTOTYPE_SPEC.md` ("The world around you"), `LOCOMOTION.md` (the gait/stamina
> contract prey plug into), and `ASSET_BIBLE.md` (available assets).
> **Scope discipline:** this brief covers only what a ≤12-animal grazing/fleeing herd
> on open Ice Age steppe, seen isometrically, opposing one persistence-hunting human,
> actually needs. It does not attempt a general ungulate-behavior survey.
>
> **Confidence tags** (per the project's scientific-honesty rule) on every quantitative
> figure: `[established]` = replicated peer-reviewed measurement · `[theory]` =
> published model/hypothesis, not directly measured · `[analogue-inferred]` = drawn
> from a living relative for an extinct/unmeasured case · `[folklore-corrected]` =
> a popular figure I am explicitly down-rating against the literature.

---

## 1. Prey species choice: Reindeer (*Rangifer tarandus*) vs. Steppe/Przewalski horse (*Equus*)

Both are period-correct. The Late Pleistocene Eurasian "mammoth steppe" was dominated by
a small number of large grazers, and reindeer and horse are two of the three keystone
species (with bison) in the classic guild described by **Guthrie (2001, *Quaternary
Science Reviews*, "Origin and causes of the mammoth steppe")** `[established]`. Both are
overwhelmingly the most-depicted prey in Upper Paleolithic cave art and dominate the
faunal assemblages of European hunter sites (**Guthrie 2005, *The Nature of Paleolithic
Art***) `[established]`. So period-correctness does **not** decide this; the other four
axes do.

| Axis | Reindeer / caribou | Steppe / Przewalski horse |
|---|---|---|
| **(a) Period-correctness** | Keystone Late-Pleistocene steppe-tundra grazer; arguably *the* iconic persistence/ambush prey of Ice Age Europe. `[established]` | Equally attested (*E. ferus / lenensis*); a mammoth-steppe keystone grazer. `[established]` | 
| **(b) Herd legibility at iso distance** | Forms **large, loose, high-cohesion aggregations**; strong collective flush; antlers give a distinctive silhouette. Reads clearly as "a herd" even as grey-box. `[established]` | Lives in **small, stable harem bands** — a stallion, ≤~10 mares + young (**Klingel's classic equid social-organization work; King et al. Przewalski studies**) `[established]`. Multi-level "herd of harems" structure is subtle and reads as a looser cluster at distance. |
| **(c) Persistence-hunt fiction** | The canonical documented persistence-hunt quarry is antelope/kudu, but any cursorial ungulate that overheats under sustained pressure fits. Cervids are a clean fit. `[analogue-inferred]` | Horses are elite endurance runners themselves (the one ungulate humans race over 100 mi). A horse is a *harder* persistence target — good later, weaker for a first honest read of the asymmetry. `[established]` |
| **(d) Asset availability** | **FREE** — ANIMAL VARIETY PACK ships Stag + Doe, 20 anims each + AnimBP (`ASSET_BIBLE.md`). Zero cost, herd-ready today (stag+doe = built-in silhouette variety). | **PAID** — MalberS Horse Animset **$74.99** (best gaits on market) or an unverified Horse Herd pack. No free herd-ready horse. |

### Recommendation: **Reindeer/caribou for Slice 2. Unambiguous.**

Three of four axes favor reindeer, and the two that matter most for *this* slice's
hypothesis (H5 = does the herd read as alive?) are decisive:

1. **Legibility is the whole game at isometric distance.** `PROTOTYPE_SPEC.md` names
   readability-at-camera-distance as the #2 prototype risk. A large, cohesive, visibly
   flushing cervid herd is the single most legible "living herd" silhouette available.
   The horse's small stable harem is biologically richer but reads as a quieter, more
   ambiguous cluster — exactly the wrong bet when you are testing whether a herd *reads*
   as alive at all.
2. **$0 and shippable today** vs. a $75 paid dependency, for a disposable hypothesis
   test. This is squarely the Asset Bible's "validate free, buy after fun is proven" rule.
3. **The persistence asymmetry is cleaner against a cervid.** A horse's own endurance
   muddies the "you can never win the sprint, only outlast" fiction that Slice 2→4 exists
   to prove. Save the horse (and its superb MalberS gaits) for a later species slice,
   per the Asset Bible acquisition order.

The only thing the horse wins on is emergent social depth — and that is a Stage-4
luxury, not a Slice-2 requirement.

---

## 2. Herd behavior: cohesion, collective vigilance, alarm propagation, and FID

**Cohesion & coordination.** Ungulate herds are not rigid formations; they are loose,
polarized aggregations that tighten under threat. Movement is led disproportionately by a
few individuals — in equids, a mature mare typically leads routine movement while the
stallion drives from the rear (**King & Gurnell / Klingel equid work**) `[established]`.
Design implication: a herd needs weak, emergent leadership (a few higher-weight
individuals others follow), not an assigned "leader" flag.

**The many-eyes / collective-vigilance effect** — the single most important behavior to
capture, because it is what makes a herd feel *smarter than one animal*. Per-capita
vigilance **falls as group size rises**, freeing each animal to graze more: the "group-size
effect," originating with **Pulliam (1973, *Journal of Theoretical Biology*, "On the
advantages of flocking")** `[theory]` and replicated across ungulates and equids —
including directly in Przewalski/khulan (*Equus*) herds (**e.g. the Tibetan wild ass /
khulan group-size-vigilance studies, *Current Zoology* 2021**) `[established]`. Two
distinct mechanisms both hold: **detection** (more eyes spot the threat sooner → larger
groups flush at *greater* distance) and **dilution/safety-in-numbers** (each individual
feels safer → may tolerate a *closer* approach). **Stankowich (2008, *Biological
Conservation*, "Ungulate flight responses to human disturbance: a review and
meta-analysis")** `[established]` finds larger groups generally flee at greater distances
(detection dominating), and that groups containing young, or in open habitat, flee
earlier still. Cheap implementable proxy: each prey's effective detection radius scales
mildly **up** with the number of herdmates within cohesion range (collective detection),
while individual "nervousness" ticks **down** (dilution) — the two are separable knobs.

**Alarm / flush propagation.** A herd does not flee by each animal independently seeing
the threat; the flush is *contagious*. One animal's alarm posture or bolt is a signal
that neighbors copy within a fraction of a second, producing a wave far faster than
independent detection would. This is the same class of behavior as startle-wave
propagation in flocks (**Reynolds 1987**; empirically, escape waves propagate at
near-constant speed through fish schools and bird flocks — **Procaccini et al. 2011,
*Animal Behaviour*, starling "agitation waves"**) `[established]`. Design implication: a
single detection event should inject an *alarm signal* that spreads neighbor-to-neighbor,
not a global "herd panics" flag. This is the cheapest, highest-payoff realism win in the
slice — it is what makes the flush read as a herd reacting rather than N animals reacting.

**Flight-Initiation-Distance (FID) as a decision surface, not a constant.** The single
most important framing for Slice 3. FID is the predator-distance at which the animal
bolts, and the entire literature treats it as **context-dependent** — an economic decision
balancing predation risk against the cost of fleeing (**Ydenberg & Dill 1986, *Advances in
the Study of Behavior*, "The economics of fleeing from predators"**; formalized for
approach by **Blumstein 2003**) `[established]`. In ungulates specifically, **Stankowich
(2008)** `[established]` shows FID rises with: approach directness and speed, open habitat
(less cover), presence of young, larger groups, and prior harassment. It falls with:
predictable/tangential approach, available cover, and habituation. **Concrete steppe
number:** reindeer/caribou FID to an approaching human runs roughly **50–350 m** depending
on wildness, terrain, and disturbance history (**Reimers & colleagues, *Rangifer* /
wild-reindeer disturbance studies**) `[analogue-inferred for our exact steppe context]` —
wide by design, because it is a surface, not a scalar. Design implication: FID must be a
*function* (approach angle × approach speed × herd size × has-been-pressed), and the human
learning to manipulate it — approach obliquely and slowly to close distance before the
flush — is precisely the "read the situation" decision the prototype wants (Success
Criterion #5).

---

## 3. Boids as substrate: where Reynolds is enough, and where biology must override

**Reynolds (1987, *SIGGRAPH*, "Flocks, herds and schools: a distributed behavioral
model")** `[established]` gives three local steering rules — **separation** (avoid
crowding neighbors), **alignment** (match neighbors' heading), **cohesion** (steer toward
local centroid) — each computed only over neighbors within a radius (and, more faithfully,
a forward field-of-view). For ≤12 animals this is trivially cheap (O(n²) is nothing at
n=12; no spatial hash, no ECS, per spec).

**Minimal parameter set that produces believable *grazing*:**
- Low max speed (wander/graze gait, well below jog), frequent brief stops (graze pauses).
- **Cohesion weight low-to-moderate**, **separation weight moderate** (personal space —
  bigger than the flee spacing), **alignment weight low** (grazing herds are only weakly
  polarized; too much alignment looks like a marching band).
- A slow **wander** term (Perlin/randomized heading drift) so the herd meanders and
  reshapes — this is what sells "alive and idle" more than the three core rules do.
- A weak **home/resource bias** so the herd grazes an area rather than drifting off the map.

**Minimal changes that produce a believable *coordinated flee*:** flip the same weights
rather than switching systems —
- **Alignment weight spikes** (fleeing herds are highly polarized — everyone points the
  same way). This single change is most of what makes a flush look coordinated.
- **Cohesion stays moderate** (they flee *together*), **separation drops slightly** (they
  bunch — the selfish-herd effect, **Hamilton 1971, "Geometry for the selfish herd"**)
  `[theory]`, **max speed jumps to the flee gait**.
- Add the **predator-directed flight vector** below.

**Where real biology must OVERRIDE naive boids** (naive boids alone will read as "dumb
particles" — the explicit failure condition in the spec):

1. **Predator-directed flight vector.** Pure boids has no predator term. Add an
   away-from-threat steering force whose weight scales with proximity/FID breach. Real
   prey flee to *safety* (cover, open escape lanes, uphill for cervids), not merely
   directly away — but a directed-away vector plus terrain steering is the honest minimum.
   Without this, the flush has no *reason* and no direction.
2. **Stragglers / heterogeneity.** Identical agents flee identically — the tell-tale
   "particle" look, and fatal to the persistence fiction, which *requires* a lagging
   target. Give each animal small per-individual variation in max speed, stamina, reaction
   latency, and nervousness. The persistence hunt is won against the straggler this
   heterogeneity produces — it is a mechanic, not flavor.
3. **Herd splitting & the abandoned individual.** Under sustained directional pressure a
   real herd fragments; a pressed individual gets cut from the group and, isolated, cannot
   benefit from many-eyes. Cohesion must be *breakable* — when an animal is far enough or
   panicked enough, it drops herd cohesion and runs solo. This is the moment the hunt
   becomes winnable and is the emotional core of Slice 4.
4. **Panic decay / return-to-graze.** Boids has no emotional state. Prey that escape far
   enough, or stop being pressed, must cool down and re-aggregate to grazing (spec
   explicitly requires this). A per-agent alarm scalar that decays over time drives the
   graze↔flee weight blend — one number, and it also carries the alarm-propagation signal
   from §2.

Net: **one boids core, two weight profiles (graze/flee) blended by a per-agent alarm
scalar, plus a predator vector and per-agent heterogeneity.** That is the whole substrate.

---

## 4. Prey energetics for the persistence-hunt asymmetry

The asymmetry the spec demands — **prey sprint faster than the human but tire faster and
recover poorly while pressed** — is not a game contrivance; it is the actual biomechanical
basis of the human endurance-running hypothesis.

**Bramble & Lieberman (2004, *Nature*, "Endurance running and the evolution of *Homo*")**
`[established]` (the paper the project already cites) argues *Homo* is anatomically
specialized for sustained running — nuchal ligament, spring-tendon legs, large gluteus
maximus, and above all **thermoregulation by sweating over near-hairless skin**, decoupled
from breathing. **Carrier (1984, *Current Anthropology*, "The energetic paradox of human
running and hominid evolution")** `[established]` frames the core asymmetry: **quadruped
cursors cool primarily by panting, which is mechanically coupled to their gallop stride**
— a galloping animal cannot pant freely and so cannot dump heat while sprinting. Forced to
keep running in the heat, it drives its core temperature up and must stop to avoid
hyperthermia. The human, sweating and pacing, does not.

**The persistence hunt is the direct consequence.** Documented Kalahari persistence hunts
(**Liebenberg 2006, *Journal of Human Evolution*, "Persistence hunting by modern
hunter-gatherers"**) `[established]` run a kudu/antelope to hyperthermic collapse over
**~2–5 hours and 25–35 km at ~40 °C** — the human never wins on speed, only by denying the
animal the standing-still time it needs to cool and recover. The prey is *faster in every
individual dash* and still loses.

**Numbers for the asymmetry** (all as inputs to the `LOCOMOTION.md` gait/stamina model):
- **Prey burst speed >> human.** Reindeer/caribou top gallop ~**60–70 km/h**
  `[folklore-corrected]` — popular sources claim 80 km/h; peer-reviewed/observed wild
  running clusters at **40–65 km/h**, so tune to the low end. Sustainable escape gallop
  ~**30–35 km/h**, cruising trot ~**15–20 km/h** for long distances `[established]`. The
  human's sprint (620 cm/s ≈ 22 km/h in the current human config) is **below the prey's
  sustainable gallop** — correct: you can never win the dash.
- **Prey tire fast, recover slowly *while pressed*.** This is the knob that makes the hunt
  winnable. In the gait model this is a **large negative sprint stamina delta** and a
  **rest-regen that only pays out when nearly stationary** — a pressed animal that is never
  allowed to stop never regains its burst. This mirrors the `LOCOMOTION.md` "Large
  herbivore" sketch: danger lives in *acceleration and inability to recover*, not in a
  bottomless sprint. `[theory / analogue-inferred]` — no clean cervid stamina-curve exists;
  this is grounded in the Carrier/Liebenberg mechanism, then tuned by feel.
- **The human's edge is the signed positive jog delta** (`LOCOMOTION.md`: human recovers
  *while jogging*). The prey has no positive-recovery gait. That structural difference —
  not any single speed number — *is* the persistence hunt. H14 (compressing hours to
  minutes) is a tuning problem on top of a biomechanically honest foundation.

---

## Tuning table — FIRST-PASS GUESSES ONLY (playtest-tune everything; none is canon)

> Every value below is a **starting point for iteration**, not a design commitment.
> Speeds are in cm/s to match `LOCOMOTION.md`/`UAnimalConfig`. Prey plug into the exact
> same gait/stamina model as the human. Expect to move all of these in playtest.

### Prey locomotion (reindeer `UAnimalConfig`, mirrors `LOCOMOTION.md` sketch style)

| Field | First-pass guess | Rationale (all tunable) |
|---|---|---|
| Walk (graze) | 180 cm/s, +6/s stamina | slow graze gait; recovers only near-still |
| Jog (escape cruise) | **520 cm/s**, −2/s | faster than human jog (400); the sustainable flee |
| Sprint (dash) | **780 cm/s**, −16/s, accel 900 | > human sprint (620) — dash is unwinnable; empties in ~7 s |
| Preferred gait | Walk | grazes by default |
| Turn rate | 300°/s | more maneuverable than human; cornering is their defense |
| Rest regen | +18/s (only < 20 cm/s ground speed) | **the crux**: pressed animal never gets it |
| Max stamina | 100 | same scale as human for readability |

### Herd & perception (Slice 2–3)

| Parameter | First-pass guess | Notes |
|---|---|---|
| Herd size | **8** (spec range 6–12) | mid of range; stag+doe mesh mix for silhouette variety |
| Neighbor radius (boids) | ~800 cm | who counts as a herdmate |
| Cohesion / Separation / Alignment (GRAZE) | 0.6 / 1.2 / 0.3 | separation > cohesion; weak alignment = idle meander |
| Cohesion / Separation / Alignment (FLEE) | 0.8 / 0.9 / **1.8** | alignment spikes = coordinated flush |
| Wander weight (graze) | 0.4 | the "alive and idle" term |
| Detection radius (base) | 2500 cm | line-of-sight only this slice (no scent — Stage 2) |
| Detection scaling w/ herd size | +5%/herdmate in range, cap +40% | many-eyes detection (Pulliam) |
| Individual nervousness | −4%/herdmate, floor −30% | dilution (separable from detection) |
| **FID (base, direct fast approach)** | 1400 cm | the bolt threshold; a **function**, not constant |
| FID × oblique/slow approach | ×0.5 | rewards the player's "read & approach" skill |
| FID × has-been-pressed | ×1.4 | wary animals flush earlier |
| Alarm propagation | neighbor copies within 0.2–0.4 s | contagious flush, not global flag |
| Alarm decay → return-to-graze | ~8–15 s of no pressure | then re-aggregate & graze |
| Per-agent heterogeneity | ±8% speed/stamina, ±0.3 s reaction | **creates the straggler** — do not skip |
| Cohesion-break (split) threshold | panic high AND >1500 cm from herd | isolated animal runs solo = the winnable target |

### Persistence-hunt sanity check (the three numbers that must hold)

- Human jog (400) **>** prey graze/recover walk (180) → you close on a resting/grazing animal. ✓
- Human jog (400) **<** prey escape jog (520) → a calm fleeing herd pulls away; you cannot chase the whole herd. ✓
- Prey sprint (780) **>** human sprint (620), but empties in ~7 s with near-zero recovery-while-moving → deny it rest and its burst is gone; the human's positive jog delta outlasts it. ✓ **This is the hunt.**

---

## Sources (named, for verification)

- Bramble D.M. & Lieberman D.E. 2004. *Endurance running and the evolution of Homo.* Nature 432:345–352.
- Carrier D.R. 1984. *The energetic paradox of human running and hominid evolution.* Current Anthropology 25:483–495.
- Liebenberg L. 2006. *Persistence hunting by modern hunter-gatherers.* Journal of Human Evolution / Current Anthropology.
- Reynolds C.W. 1987. *Flocks, herds and schools: a distributed behavioral model.* SIGGRAPH Computer Graphics 21(4):25–34.
- Pulliam H.R. 1973. *On the advantages of flocking.* Journal of Theoretical Biology 38:419–422.
- Hamilton W.D. 1971. *Geometry for the selfish herd.* Journal of Theoretical Biology 31:295–311.
- Ydenberg R.C. & Dill L.M. 1986. *The economics of fleeing from predators.* Advances in the Study of Behavior 16:229–249.
- Blumstein D.T. 2003. *Flight-initiation distance in birds is dependent on intruder starting distance.* Journal of Wildlife Management 67:852–857.
- Stankowich T. 2008. *Ungulate flight responses to human disturbance: a review and meta-analysis.* Biological Conservation 141:2159–2173.
- Procaccini A. et al. 2011. *Propagating waves in starling flocks under predation.* Animal Behaviour 82:759–765.
- Klingel H. (equid social organization); King S.R.B. et al. (Przewalski horse social structure & movement). 
- Guthrie R.D. 2001. *Origin and causes of the mammoth steppe.* Quaternary Science Reviews 20:549–574; Guthrie 2005, *The Nature of Paleolithic Art.*
- Reimers E. et al. (wild reindeer / Rangifer flight-response & disturbance studies, *Rangifer*).
- Group-size vigilance in *Equus*: khulan / Tibetan wild ass studies, *Current Zoology* 67(1) 2021.
