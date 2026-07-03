# PROJECT STATUS — The First Life

> Maintained by the Chief Architect. The single source of truth for *where the project actually is*.
> Companion files: `CURRENT_SPRINT.md` (what we do next), `TASK_GRAPH.md` (dependencies & parallelism),
> `ARCHITECTURE_DECISIONS.md` (engineering ADRs), `IMPLEMENTATION_LOG.md` (per-loop history).
> Design law lives in `docs/` (`VISION.md`, `GAME_DNA.md`, `DESIGN_DECISIONS.md`, `HYPOTHESES.md`).
> Last updated: 2026-07-03 — orchestration bootstrap.

---

## 1. One-paragraph state

A compiling, single-player UE 5.6 grey-box slice runs on Mac: an early *Homo sapiens* body with a
three-gait endurance-locomotion + stamina economy, a clean AI↔player possession seam, a display-only
per-species perception switcher (keys 1–4), a canvas HUD, and a script-built greybox map. **Nothing has
been validated by a human hand yet.** The project is not blocked on engineering capacity — it is blocked
on a **hands-on feel-pass**, a **binary-asset (mannequin) import**, and **three design sign-offs** on the
perception/cognition architecture. The next code worth writing is gated behind those.

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
| HUD | `AFirstLifeHUD` | Canvas-drawn (no UMG): stamina bar, gait label, possession prompt, perception readout. |
| World | `Scripts/build_greybox_map.py` → `GreyBox.umap` | 100×100 m floor, walls, 3 rocks + a rise, movable sun/skylight. Lumen disabled (Metal/macOS 26 black-Lit bug); SSR fallback. |
| Toolchain | — | UE 5.6 compiles on Mac only after Metal Toolchain install + `Apple_SDK.json` MaxVersion→26.2.0 (re-apply after every engine reinstall — see memory). |

## 3. What is STUBBED / placeholder (known, intentional)

- **AI brain** — `AAnimalAIController` is a random-wander stub. No behavior tree, blackboard, or state machine. No perception hookup. By design for Slice 1.
- **Herd** — does not exist. GameMode spawns 3 hardcoded wander targets purely so the perception switcher has something to see; explicitly **not** the herd (H5).
- **Prey, hunger, scent-as-gameplay, day/night, weather, injury, combat, crafting, multiplayer** — none exist. Scope wall held.
- **Meshes** — engine-cube grey-box body/head. The believable human mesh is unacquired (binary-asset GUI work).
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

- **GATE-A — Hands-on feel-pass (H2).** Someone must play 10 min and tune `DA_Human` live. Validates the endurance/stamina rhythm. *Cannot be delegated to a headless agent.*
- **GATE-B — Perception/cognition sign-off.** `SPECIES_COGNITION.md`, `PERCEPTION_DESIGN.md`, `SPECIES_KNOWLEDGE_FOG.md` are stamped *"awaiting agreement — no code."* One clean sign-off (four blockers, see `CURRENT_SPRINT.md`) unlocks the Umwelt Slice 1.
- **GATE-C — Mannequin/human mesh acquisition.** Fab/Epic GUI work (~15 min) to replace grey-box cubes. Binary asset — *cannot be authored as text.* Unblocks presence-quality playtests (H3/H18).

## 7. Hypotheses in flight

- **Testing now (need play):** H1, H2, H5-prep, H14 (persistence-hunt pacing), H15/H18 (camera).
- **Live but display-only:** H17 (species perception reads) — slice-1 gating built; sharper question is "does losing information feel like a body or a cheat?"
- **Deferred (do not build for):** H6, H8, H9–H13 — see `HYPOTHESES.md` Tier 2.

## 8. Roadmap position

Roadmap **Stage 1 (Prototype — "The First Life")**, build-order **step 1 authored** (human + stamina + camera),
**step 2 (living herd) is next**. Steps 3–5 (awareness/flee, prey stamina + feed, hunger) follow. The
persistence-hunt core loop (step 4) is the make-or-break (H14). Perception/cognition (the Umwelt) is a
parallel design-gated track, not on the prototype's critical play-loop path.
