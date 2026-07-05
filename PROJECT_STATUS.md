# PROJECT STATUS — The First Life

> Maintained by the Chief Architect. The single source of truth for *where the project actually is*.
> Companion files: `CURRENT_SPRINT.md` (what we do next), `TASK_GRAPH.md` (dependencies & parallelism),
> `ARCHITECTURE_DECISIONS.md` (engineering ADRs), `IMPLEMENTATION_LOG.md` (per-loop history).
> Design law lives in `docs/` (`VISION.md`, `GAME_DNA.md`, `DESIGN_DECISIONS.md`, `HYPOTHESES.md`).
> Last updated: 2026-07-04 — post dev-loop iters 1–6 + asset/animation pass.

---

## 1. One-paragraph state

A compiling, single-player UE 5.6 slice runs on Mac: an early *Homo sapiens* body with a three-gait
endurance-locomotion + stamina economy, a clean AI↔player possession seam, and a **complete, winnable,
tracked persistence-hunt vertical slice (H14)** — hunger gives the reason to hunt, an 8-reindeer and a
12-saiga herd (graze/flee brains) give two contrasting quarries, per-agent body condition produces the
catchable straggler, exhaustion + interruptible dwell is the catch, feeding delivers the prey's nutrition,
and a scent-spoor arrow keeps a lost quarry trackable. An **F1 telemetry overlay** instruments it all for
the feel-pass. On top of that, a first **visual-asset pass** landed: imported deer/tiger/wolf skeletal
meshes (+ a static human mesh), a C++ speed-driven animation driver that plays each species' idle/walk/run
clips with no AnimBP required, runtime species/body swapping, ground snapping, a Megascans tundra
`Tundra.umap`, and wolf/tiger validation pairs spawned through the same species-as-data route. **Nothing
has been validated by a human hand yet** — everything is compile-green only, which makes the still-open
GATE-A feel-pass the unambiguous bottleneck. Remaining non-engineering gates: the feel-pass itself,
perception/cognition design sign-off (GATE-B), and editor-side environment work (Landscape pass per
`docs/ENVIRONMENT_TUNDRA.md`).

---

## 2. What is BUILT (authored, compiles green, unverified by play)

| System | Class(es) | State |
|---|---|---|
| Species-as-data | `UAnimalConfig` + `USpeciesPerceptionProfile` (both `UPrimaryDataAsset`), one pawn `AAnimalCharacter` | **Solid.** D13 honored — a species is two data assets, never a subclass. |
| Locomotion (3 gaits) | `ULocomotionComponent` + `LocomotionTypes.h` | **Solid architecture.** Walk/Jog/Sprint, data-driven speeds/accel/turn/brake; ticks `TG_PrePhysics`; sprint re-negotiated vs stamina each frame. Numbers are first-pass guesses. |
| Energy economy | `UStaminaComponent` | **Solid.** Tickless signed-rate reservoir + exhaustion latch; gait-agnostic (heat/hunger can reuse it). |
| Possession seam | `AAnimalCharacter` (AIControllerClass + AutoPossessAI) ↔ `AWolfPlayerController` ↔ `AAnimalAIController` | **Solid & demonstrated.** `P` physically hands the body between AI and player. Strong early H7 evidence. Not authority-safe (no netcode — out of scope). |
| Perception (display-only) | `USpeciesPerceptionComponent` + `USpeciesPerceptionProfile` | **Proto-slice-1.** Vision-only knowledge gating: targets are **Tracked** (shown) or **Absent** (hidden), 0.4 s persistence. A real but embryonic belief store (`HiddenTargets`, `SecondsSinceSeen`). Runs **only for the player body.** 4 species seeded in code. |
| Scent field (placeholder) | `UScentFieldSubsystem` | **"FAKE, on purpose."** Flat FIFO `TArray<FScentPoint>` (loc+time), 75 s age-out, fixed wind drift. Not a grid, no diffusion/terrain/odor identity. Read-only overlay consumer. |
| Hunt loop (H14) | `UHuntSubsystem` | **Built (iter 1).** Catch = interruptible ~1.5 s dwell within 200 cm of an *exhausted* prey (the exhaustion latch IS the catch window — capture-myopathy grounding); time-to-kill clock on HUD; feed = `Refill()` + prey's `NutritionValue`. Continuous fatigue head-drop + downed-collapse posture telegraph. |
| Hunger | `UHungerComponent` | **Built (iter 2).** Tickless reservoir, ~20 min to empty; starvation scales stamina regen (floor 0.3) and capacity (floor 0.5) via two opaque scalars. Player-only pressure; nutrition lives on the eaten animal (reindeer 85, saiga 48). |
| Two prey species | `CreateReindeerConfig` / `CreateSaigaConfig` | **Built (iters P2+3).** 8-reindeer loose herd (+X) vs 12-saiga tight cluster (−X): saiga dashes faster, out-cruises, but shallow fast-refill tank — win by tempo, not distance. Herd identity = config pointer. |
| Catchable straggler | per-agent condition (`SetConditionOverride`, `SetSpeedScale`) | **Built (iter 4).** Condition `[0.78..1.0]`, right-shouldered roll; fully scales stamina, lightly trims speed — the poor animal self-sorts to the rear under pressure. Makes H14 winnable. |
| Lose-and-track spoor | `UScentFieldSubsystem::GetFreshestTrailNear` | **Built (iter 5).** Player lays no scent, so fresh points are prey spoor; HUD draws a HOT→COLD arrow only when the nearest prey is beyond reveal distance. |
| Playtest telemetry | `AFirstLifeHUD::ToggleDebugOverlay` (**F1**) | **Built (iter 6).** Per-animal condition/stamina/state overlay, straggler flagged red. Dev-only, off by default. Built to make GATE-A productive. |
| Herd brain | `FHerdBrain` + `AAnimalAIController` | **Built (Slice 2).** Boids graze/flee, contagious flush, FID decision surface, per-agent jitter → emergent behavior. No behavior trees. |
| Visual assets + animation | `Content/Animals/*`, C++ anim driver in `AAnimalCharacter` | **Built (2026-07-03/04).** Deer/tiger/wolf skeletal meshes with idle/walk/run clips; human = static mesh (no anim). `UpdateLocomotionAnim` plays clips by ground speed (hysteresis, gait bands from the species' own config) whenever `VisualAnimClass` is unset — an AnimBP upgrades it for free later. Runtime species/body swap + ground snapping. Wolf/tiger pairs spawned as normal species configs to validate the import route. |
| HUD | `AFirstLifeHUD` | Canvas-drawn (no UMG): stamina bar, hunger bar, gait label, possession prompt, perception readout, time-to-kill, spoor arrow, F1 overlay. |
| World | `Scripts/build_greybox_map.py` → `GreyBox.umap`; `Tundra.umap` | Greybox: 100×100 m floor, walls, rocks + rise, movable sun/skylight. Lumen disabled (Metal/macOS 26 black-Lit bug); SSR fallback. Tundra: Megascans terrain slab — **reads dead; Landscape rework planned** (`docs/ENVIRONMENT_TUNDRA.md`). |
| Toolchain | — | UE 5.6 compiles on Mac only after Metal Toolchain install + `Apple_SDK.json` MaxVersion→26.2.0 (re-apply after every engine reinstall — see memory). |

## 3. What is STUBBED / placeholder (known, intentional)

- **AI brain** — ✅ **Slice 2 built (2026-07-03 D).** `AAnimalAIController` now runs a herd brain: a pure
  per-agent `FHerdBrain` (boids graze/flee + contagious flush + FID decision surface + emergent straggler)
  fed by proximity sensing (`SenseHerd()`, the future U4/Umwelt seam). No behavior tree/blackboard — a
  hand-rolled alarm-scalar state machine, which is enough at this scale. Unverified by play; numbers are
  first-pass guesses awaiting the owner's tuning pass.
- **Herd** — ✅ **Slice 2 built (2026-07-03 D).** `AFirstLifeGameMode` spawns 8 reindeer `AAnimalCharacter`s
  (runtime `CreateReindeerConfig()` — data, not a subclass) via the `SetConfigOverride` seam; they graze,
  flush contagiously, and flee as a group. Replaces the 3 debug wander targets. Grey-box cubes; H5 validated
  in code, not yet by a human hand.
- **Day/night, weather, injury, combat, crafting, multiplayer** — none exist. Scope wall held.
  (Prey, hunger, and scent-as-tracking HAVE since been built — see §2.)
- **Human mesh** — a static mesh (`asian_old_man_warrior...`), so it cannot animate; a skeletal human
  (e.g. Game Animation Sample mannequin) is still the path to presence-quality playtests (H3/H18).
- **Animation Blueprints** — none exist; the C++ clip driver covers idle/walk/run. ABPs (blend spaces,
  state machines, wolf `sniffing`, turn-in-place) are an optional editor upgrade (`docs/ANIMATION.md`).
- **Tundra environment** — a dead photogrammetry slab; needs the sculpted-Landscape Stage 1 pass
  (editor GUI work, `docs/ENVIRONMENT_TUNDRA.md`).
- **Perception post-process** — placeholder global tint/vignette; directional effects need a post-process material (binary asset).
- **`DA_Human.uasset`** — predates the `FGaitSettings` restructure; orphaned flat floats ignored, class defaults are the human profile (behaviour identical). Needs a re-save to clean.

## 4. Architecture — where we are vs the target

**Target (canonical, from `docs/SPECIES_COGNITION.md` §5): the Umwelt four-layer.**

```
WORLD (sim truth) → ① STIMULUS FIELD (world's decaying memory) → ② CHANNELS (per-species sampling)
                                       → ③ UMWELT (per-agent belief store) → ④ CONSUMERS
                                                                              ├─ AI brain (reads as data)
                                                                              └─ Player renderers (Knowledge Fog)
```

**What exists today maps onto it as an embryo, wrongly coupled:**

- ① Stimulus Field → seeded by `UScentFieldSubsystem` (one channel's worth, flat array). Needs generalising into a species-agnostic append-and-decay field.
- ② Channels → only a Vision channel exists, inlined inside `USpeciesPerceptionComponent::VisionChannelSees`.
- ③ Umwelt → exists in embryo (`HiddenTargets` + `SecondsSinceSeen`, 2 rungs) but **lives inside the player's perception component**, not as a standalone per-agent store.
- ④ Consumers → only the **player renderer** consumes it (via hide/show). The **AI brain does not consume any belief store** — the "one model, two consumers" invariant (D7 extended to senses) is *not yet realised*.

**The single most important architectural move ahead:** extract the belief store into a per-agent **Umwelt** that both the AI brain and the player renderer read — turning perception from player-only display into the shared simulation substrate the vision requires. This is design-gated (see §6, GATE-B).

## 5. Technical debt (tracked, none blocking)

| Item | Severity | Note |
|---|---|---|
| `AWolfPlayerController` misnamed (drives the human) | Low | Rename → `APossessionPlayerController` queued for a "natural refactor window"; touches input seam, do after build is hand-validated. |
| `DA_Human.uasset` orphaned fields | Low | Re-save in editor; behaviour already correct. |
| `DA_Wolf.uasset` stale on disk | Trivial | Unreferenced; delete freely. |
| Belief store coupled to player perception | **Medium (architectural)** | Blocks the "AI reads Umwelt" payoff; resolved by the Umwelt extraction (design-gated). |
| Possession toggle not authority-guarded | Low (deferred) | Only matters at a networked milestone — not a goal. TODO left in code. |
| `SecurityToken` in `DefaultEngine.ini` | **Fixed 2026-07-03** | AndroidFileServer template leftover removed (Desktop-only project). |
| clangd false errors (no `compile_commands.json`) | Trivial | IDE-only; real UBT build is green. |

## 6. Active blockers / gates (this is the real bottleneck)

The project cannot productively spawn *implementation* agents until these clear. They are owner (vision-holder) decisions and human-in-the-loop tasks, not engineering capacity:

- **GATE-A — Hands-on feel-pass (H2 + H14).** *Still open, now with much higher stakes:* the full hunt
  loop (hunger → chase → straggler → catch → track → feed) is built on ~six commits of first-pass guesses.
  Play ~20 min with the F1 overlay on; tune live. *Cannot be delegated to a headless agent.* **This is the
  single bottleneck — the dev loop was deliberately paused here (iter 6).**
- **GATE-B — Perception/cognition sign-off.** `SPECIES_COGNITION.md`, `PERCEPTION_DESIGN.md`, `SPECIES_KNOWLEDGE_FOG.md` are stamped *"awaiting agreement — no code."* One clean sign-off (four blockers, see `CURRENT_SPRINT.md`) unlocks the Umwelt Slice 1.
- **GATE-C — Believable bodies. ~80 % cleared (2026-07-03/04):** deer/tiger/wolf skeletal meshes imported
  and animating via the C++ clip driver. **Remaining:** a *skeletal* human (current human mesh is static →
  cannot animate), plus optional per-species AnimBPs. Editor GUI work, `docs/ANIMATION.md`.
- **GATE-D (new) — Living tundra Landscape.** `Tundra.umap`'s Megascans slab reads dead. Stage 1
  (sculpted Landscape + layered material + low sun/fog + hand scatter) is editor GUI work, a few hours,
  spec'd in `docs/ENVIRONMENT_TUNDRA.md`. Not blocking GATE-A (greybox suffices for the feel-pass).

## 7. Hypotheses in flight

- **Testing now (need play):** H1, H2, H5-prep, H14 (persistence-hunt pacing), H15/H18 (camera).
- **Live but display-only:** H17 (species perception reads) — slice-1 gating built; sharper question is "does losing information feel like a body or a cheat?"
- **Deferred (do not build for):** H6, H8, H9–H13 — see `HYPOTHESES.md` Tier 2.

## 8. Roadmap position

Roadmap **Stage 1 (Prototype — "The First Life")**, build-order **steps 1–5 authored** (human + stamina +
camera; living herds; awareness/flee; persistence-hunt catch + feed; hunger) — the entire prototype
play-loop is code-complete but **zero-validated**. The make-or-break H14 question ("is running an animal
to exhaustion fun for 20 minutes?") is now answerable by playing, not by building. Perception/cognition
(the Umwelt) remains a parallel design-gated track (GATE-B), not on the critical play-loop path.
Presentation (skeletal human, AnimBPs, living tundra Landscape) is a parallel editor-side track.
