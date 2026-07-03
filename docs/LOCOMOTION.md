# Locomotion Architecture — Movement as a Core Pillar

> Status: implemented for Homo sapiens (Slice 1.5, 2026-07-03). Companion to
> `PROTOTYPE_SPEC.md` (scope) and `HYPOTHESES.md` (H2 energy economy, H7 possession
> seam, H14 persistence hunt). This document is the contract future species plug into.

## Philosophy

Movement is not traversal between the interesting parts — it **is** the interesting
part. Every species is a different answer to the same physics problem (move a body,
spend energy, don't die), and the locomotion system is where that answer becomes
gameplay. Two rules follow:

1. **A species is a movement philosophy, not a stat block.** The wolf is not "a fast
   human"; the mammoth is not "a slow wolf". The architecture must let each species
   feel *categorically* different while sharing one state machine.
2. **All of it is data.** Species live in `UAnimalConfig` Data Assets. Adding a
   species must never require a new character class (H7); tuning must never require
   a recompile.

## The three gaits

Every land species expresses the same three intentional gaits — what differs is the
numbers each gait carries and which gait the body prefers:

| Gait | Meaning | Homo sapiens |
|---|---|---|
| **Walk** | Slow, deliberate — the stalk/recover gait | 200 cm/s, +10 stamina/s |
| **Jog** | The cruising gait; the species' travel identity | 400 cm/s, **+4 stamina/s** |
| **Sprint** | Maximum effort, bought with stamina | 620 cm/s, fast accel, −12 stamina/s |

**The human's defining trait is the signed jog delta.** Homo sapiens is the animal
that recovers *while moving* — slower than walking, far slower than standing still,
but the distance keeps accumulating. Jog is how you travel; sprint is a tactical
purchase (~8 s to empty from full); burning out locks sprint until stamina recovers
to 30%. The optimal strategy the numbers teach is the species' real biology:
endurance, not speed.

Resting is not a fourth gait but a detected condition: below 20 cm/s ground speed
the body earns the rest regen rate (+22/s for the human) regardless of what gait is
selected. You cannot "sprint" standing still.

## Architecture

```
Controller (player OR AI — H7 seam)          intent only: WantsToSprint / WantsToWalk
        │
        ▼
AAnimalCharacter                             body wiring; owns no movement logic
        │
        ▼
ULocomotionComponent                         THE movement system
   ├─ reads   UAnimalConfig                  the species' locomotion profile (data)
   ├─ drives  UCharacterMovementComponent    MaxWalkSpeed / MaxAcceleration /
   │                                         RotationRate / BrakingDeceleration
   └─ drives  UStaminaComponent              pure energy reservoir + exhaustion latch
```

Per-frame order inside `ULocomotionComponent::TickComponent` (TG_PrePhysics):

1. **Charge for reality.** Sample actual ground speed; if resting, apply rest regen,
   otherwise apply the *active* gait's signed stamina delta. The body pays for the
   frame it just lived, never for a stale intent.
2. **Re-negotiate.** Resolve intent against the new energy state:
   `Sprint` if wanted *and* `Stamina->CanSprint()` — sprint dies mid-stride at
   exhaustion, it is a continuous negotiation, not a latch (H2). Else `Walk` if
   wanted. Else the species' `PreferredGait`.
3. **Apply to the body.** Write the resolved gait's speed and acceleration to the
   CharacterMovementComponent (turn rate and braking are species constants applied
   at Configure time).

`UStaminaComponent` deliberately knows nothing about gaits: it is one battery with a
signed-rate `Update()` and the exhaustion latch. Locomotion is its first consumer;
heat, cold, and hunger can push rates into the same reservoir later without touching
locomotion.

## The species profile (`UAnimalConfig`)

Requested design values → where they live:

| Design value | Field | Notes |
|---|---|---|
| Walk / Jog / Sprint speed | `Walk/Jog/Sprint.MaxSpeed` | per-gait `FGaitSettings` |
| Acceleration | `Walk/Jog/Sprint.Acceleration` | per-gait: sprint explosiveness and herbivore slow build-up both live here |
| Turn rate | `TurnRateDegPerSecond` | low = momentum, poor maneuverability |
| (Braking) | `BrakingDeceleration` | low = the body overshoots; mass feels real |
| Max stamina | `MaxStamina` | |
| Stamina regeneration | `RestRegenPerSecond` | while (nearly) still |
| Stamina regen while moving | `Walk/Jog.StaminaDeltaPerSecond` | signed; the core species-identity knob |
| Burst duration | *emergent* | `MaxStamina / |Sprint.StaminaDeltaPerSecond|` — deliberately not a field, so it can never contradict the drain rate |
| Preferred locomotion | `PreferredGait` | the gait with no intent; AI travels at this |
| Heat generation / dissipation | *reserved* | lands as a per-gait `HeatPerSecond` in `FGaitSettings` + species `HeatDissipationPerSecond`, feeding a future `UHeatComponent` (same battery pattern as stamina). Not added yet — no system reads heat, and dead knobs invite phantom tuning. |
| Energy efficiency | *reserved* | a species-level multiplier on all stamina deltas; today it is expressed directly in the per-gait deltas. Becomes a real field when terrain/temperature modifiers need one knob to scale them. |

## How a future species plugs in

Create a new `UAnimalConfig` asset. No new classes. Sketches (illustrative numbers —
tuning happens in the editor against real prey):

| | Walk | Jog | Sprint | Preferred | Turn | Notes |
|---|---|---|---|---|---|---|
| **Homo sapiens** | 200, +10/s | 400, **+4/s** | 620, −12/s | Jog | 540°/s | recovers while moving; wins by outlasting |
| **Wolf** | 250, +8/s | **550, 0 or −1/s** | 900, −18/s | Jog | 480°/s | fast sustained lope but *cannot recover in it* — must drop to walk/rest; hunts in relays |
| **Smilodon / big cat** | 220, +6/s | 450, −4/s | **1300, −60/s** (accel 4000) | Walk | 600°/s | seconds of violence, minutes of recovery (rest regen ~8/s); ambush or nothing |
| **Large herbivore** | 180, +6/s | **650, −2/s** (accel 300) | 850, −10/s (accel 500) | Walk | **160°/s**, braking 600 | slow to start, slow to stop, cannot corner; wins straight lines, loses turns |

The philosophy differences are *structural*, not numeric: the human is the only
profile whose jog is positive; the cat is the only one whose sprint dwarfs everything
else; the herbivore is the only one whose danger lives in acceleration and turn rate
rather than in the stamina column. The chase gameplay between any two of these
profiles falls out of the data (H14: human jog 400 > herbivore recovery walk 180,
but < herbivore sprint 850 — the persistence hunt is literally these three numbers).

## Controls (unchanged)

**WASD** move (default = jog) · **Left Shift** hold = sprint · **Left Ctrl** hold =
walk · **P** release/reclaim body to/from AI. The HUD shows the stamina bar plus the
active gait label (RESTING / WALK / JOG / SPRINT).

## Open questions (deliberately unresolved)

- **Exhaustion gait cap.** Today burnout locks sprint only; a burned-out human can
  still jog at full speed. Biologically dubious, gameplay-defensible. Decide after a
  hands-on feel pass; if capped, it becomes an `ExhaustedGaitCap` field in the profile.
- **Per-gait turn rate.** One species value for now (KISS). If sprint cornering needs
  to feel worse than walk cornering (it should, eventually), turn rate moves into
  `FGaitSettings`.
- **Terrain/slope multipliers.** Uphill jog should not be free forever. Belongs to the
  slice that adds real terrain.

## Migration note

`Content/Agents/DA_Human.uasset` predates the profile restructure (flat floats →
`FGaitSettings` structs). Its orphaned old properties are ignored; the new struct
properties fall back to class defaults, which *are* the human profile — behavior is
identical. Open and re-save (or delete and re-run `Scripts/build_greybox_map.py`) to
clean it up.
