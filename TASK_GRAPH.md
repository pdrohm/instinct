# TASK GRAPH — The First Life

> A dependency DAG, **not** a TODO list. Nodes are units of work; edges are hard dependencies.
> The purpose is to maximise *safe* parallelism while protecting the project's gates and DNA.
> Maintained by the Chief Architect. Re-optimised whenever a gate clears or a node completes.
> Legend: ✅ done · 🟢 ready (no blockers) · 🟡 blocked-by-gate · ⚪ future · 🔒 human-in-loop (undelegatable)

---

## 1. The gates (edges that cross the whole graph)

Three gates dominate the graph. Almost every implementation node hangs off one of them.

- **GATE-A** 🔒 — *Hands-on feel-pass + `DA_Human` tuning* (validates H2). Human-only.
- **GATE-B** 🔒 — *Perception/cognition sign-off* (4 blockers below). Owner decision.
- **GATE-C** 🔒 — *Mannequin/human mesh acquisition* (Fab GUI). Human-only.

**GATE-B blockers (from the design-consistency audit — all four must be answered together):**
1. Declare `SPECIES_COGNITION.md` the **canonical** spec; mark the other three "superseded-in-framing."
2. **Slice-1 scope:** Visual channel + fog grid only (Absent/Tracked), *no* Memory. (Docs recommend yes.)
3. **Fog rendering:** authorise a cheap spike to choose post-process material vs. ground decal.
4. **Build-order reorder:** confirm "AI reads the Umwelt" becomes step 3 (before Motion/Scent).
   (Deferred, non-blocking: two-decay model, generosity dial, wind model, attention postures, proxy art.)

---

## 2. The graph (by track)

### Track P — Play-loop (the prototype's critical path, Stage 1)

```
✅ P1 Human body + 3-gait locomotion + stamina + camera + possession (BUILT, unverified)
        │
🔒 GATE-A (feel-pass, tune DA_Human)              🔒 GATE-C (acquire mannequin) ──┐
        │                                                                        │
        ▼                                                                        ▼
✅ P2 Living herd (boids graze+flee) ──► 🟡 P3 Awareness tuning ──► 🟡 P4 Prey stamina + the feed ──► 🟡 P5 Hunger clock
      (H5, BUILT — green)               (H5)                       *** CORE LOOP, H14 ***           (H1)
```

- **P2..P5 depend on GATE-A** (no point tuning prey against an unvalidated player economy) and consume the herd research (R1). They are the make-or-break; **P4 is the whole prototype's crux.**
- **Presence quality of P2..P5 depends on GATE-C** (grey-box cubes suppress H3/H18 reads), but the *mechanics* do not — P2..P5 can be built and tuned against cubes, then re-judged after the mesh lands. So GATE-C gates *evaluation*, not *construction*.

### Track U — Umwelt / perception (parallel, design-gated)

```
🟡 GATE-B ──► U1 Extract Umwelt belief store (entity ledger + fog grid) + Vision channel writes into it
                     │                    (mostly-additive refactor of USpeciesPerceptionComponent)
                     ├──► U2 Fog renderer (player) — the chosen spike output
                     ├──► U3 Belief decay + Remembered ghosts
                     ├──► U4 AI brain reads the Umwelt  ◄── depends on A1 (AI brain exists)
                     ├──► U5 Motion channel + Detected proxies
                     └──► U6 Stimulus Field generalisation + Scent channel + wind  ◄── touches S1
```

- **U1 is the keystone.** Everything in Track U and the "AI reads senses" payoff (U4) hangs off it.
- **U4 is where Tracks U and A converge** — it needs both the Umwelt (U1) and a real AI brain (A1).

### Track A — AI brains (parallel once herd exists; independent of GATE-B until U4)

```
✅ A1 Herd brain (graze/alert/flee/return, boids + contagious flush + FID + straggler)  [BUILT — green]
        └──► 🟡 A2 Prey decision tuning (FID, panic, recovery)  ◄── consumes R1 (now a feel-tuning pass on live numbers)
        └──► U4 (AI reads Umwelt)  ◄── also needs U1
```

### Track S — Simulation / Stimulus Field (parallel, mostly design-gated with U6)

```
⚪ S1 Generalise UScentFieldSubsystem → species-agnostic Stimulus Field (append + decay + wind)
        └──► U6 (Scent channel reads the field)
```
- S1 is a refactor of an existing placeholder; it can be *designed* now but should not land before GATE-B (it is the Stimulus-Field layer the sign-off defines). Low priority until scent enters (build step 5 / U6).

### Track R — Research (🟢 ready now, no code, feeds the tracks above)

```
✅ R1 Herd + prey research brief  → feeds P2..P5, A1, A2   [DONE → docs/research/HERD_AND_PREY.md]
⚪ R2 Fog-rendering technique survey (post-process vs decal in UE5) → feeds GATE-B #3 / U2
⚪ R3 Scent-model literature (diffusion/decay/wind) → feeds S1 / U6   (defer until scent is near)
```

### Track X — Cross-cutting hygiene (🟢 ready, safe, non-gated)

```
✅ X0 Remove checked-in SecurityToken from DefaultEngine.ini   [DONE this session]
🟢 X1 Docs coherence: unify Umwelt terminology across the 4 perception docs (per audit)  ◄── best done WITH GATE-B
🟢 X2 Governance docs (this file, PROJECT_STATUS, CURRENT_SPRINT, ARCH_DECISIONS, AGENTS/)  [DONE/ongoing]
⚪ X3 Rename AWolfPlayerController → APossessionPlayerController  ◄── after GATE-A (touches input seam)
⚪ X4 Re-save DA_Human.uasset; delete DA_Wolf.uasset  🔒 (editor GUI)
```

---

## 3. What can run in parallel RIGHT NOW (no gate, no conflict)

These have disjoint file ownership and no unmet dependency — safe to delegate concurrently:

| Lane | Node | Owner agent | Touches | Conflicts? |
|---|---|---|---|---|
| Research | R1 (herd/prey) | `research-engineer` | `docs/research/` | none — **DONE** |
| Docs | X1 (terminology unify) | `documentation-engineer` | `docs/*PERCEPTION*/COGNITION/FOG` | none (docs only) |
| Research | R2 (fog-render survey) | `research-engineer` | `docs/research/` | none |
| Governance | X2 (these files) | Chief Architect | root `*.md`, `AGENTS/` | none |

Everything else waits on GATE-A, GATE-B, or GATE-C. **This is the honest shape of the project: the parallelism available today is research + docs, not implementation.** The implementation fan-out (Tracks P, U, A, S) opens the moment the gates clear — and the `AGENTS/` roster is pre-written so it fires immediately.

## 4. Critical path

`GATE-A (feel-pass) → P2 → P3 → P4 (H14 verdict)` is the critical path to the prototype's core question.
Track U (Umwelt) runs fully in parallel to Track P and does **not** block it — perception is a design
pillar, not part of the minimal play loop. If forced to choose one track to advance first, it is **Track P
through P4**: the whole project is gated on H14 being fun, and no amount of perception architecture rescues
a persistence hunt that reads as tedium.

## 5. Merge-conflict map (ownership is disjoint by design)

- Track P owns: `LocomotionComponent`, `StaminaComponent`, `AnimalConfig`, controllers, HUD, GameMode, map script.
- Track U owns: `SpeciesPerceptionComponent`, `SpeciesPerceptionProfile`, the new `Umwelt` object, fog renderer.
- Track A owns: `AnimalAIController` (+ new brain classes).
- Track S owns: `ScentFieldSubsystem` (→ Stimulus Field).
- **The one shared seam:** `AAnimalCharacter` (adds components/accessors). Any track adding a component to it must go through the Chief Architect (Integration) to serialise those edits — this is the single highest merge-risk file.
