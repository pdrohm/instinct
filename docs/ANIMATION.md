# Animal Animation — Speed-Driven Locomotion AnimBP

> **Status update (2026-07-04, commit `68c3dfd`):** the frozen-in-bind-pose problem below is **already
> solved in C++**. `AAnimalCharacter` now drives idle/walk/run clips directly from ground speed (with
> hysteresis) whenever `VisualAnimClass` is unset — the configs point at each species' imported clips via
> `IdleAnim`/`WalkAnim`/`RunAnim`. The AnimBP recipe below is therefore an **optional upgrade** (smooth
> blending, state machines, wolf `sniffing`), not a prerequisite. The moment an ABP exists at the expected
> path, it takes over and the C++ driver goes dormant — nothing else to change.

## Why the animals looked frozen

The imported FBX meshes (Deer, Tiger, Wolf) **came with animation clips**
(`..._walk`, `..._run`, `..._idle`, wolf `running/idle/sniffing`), but a skeletal
mesh with **no `AnimInstance`** renders in its bind pose — it slides around like a
statue. Nothing was *playing* the clips.

`AnimalCharacter.cpp` already had the seam: it calls
`SetAnimInstanceClass(ResolvedConfig->VisualAnimClass)`. The configs just never set
`VisualAnimClass`, so it was handed `nullptr` → bind pose.

## What the code now expects

Each skeletal species points at an Animation Blueprint by **soft class path**
(null-safe — nothing breaks until the asset exists):

| Species | Skeleton asset                              | AnimBP the code expects            |
|---------|---------------------------------------------|------------------------------------|
| Deer    | `/Game/Animals/Deer/Deer_001_Skeleton`      | `/Game/Animals/Deer/ABP_Deer`      |
| Tiger   | `/Game/Animals/Tiger/Tiger_001_Skeleton`    | `/Game/Animals/Tiger/ABP_Tiger`    |
| Wolf    | `/Game/Animals/Wolf/SkeletalMeshes/low_poly_wolf_Skeleton` | `/Game/Animals/Wolf/ABP_Wolf` |

> The **human** (`asian_old_man_warrior...`) is a **static mesh** — no skeleton, so it
> can't animate this way. That's expected and separate.

Movement is code-driven (`CharacterMovementComponent` + `LocomotionComponent`), so the
clips must be **in-place** (root motion OFF — the default for these imports). The AnimBP
only *reads* speed and picks the pose; it must never move the capsule.

## Build one AnimBP (repeat per species). UE menu names: English (Português)

Do the Deer first end-to-end, then copy the pattern for Tiger and Wolf.

### Step 1 — BlendSpace 1D (idle → walk → run on speed)

1. In the Content Browser, open `Content/Animals/Deer/`.
2. Right-click → **Animation → Blend Space 1D** (*Animação → Espaço de Mesclagem 1D*).
3. Pick skeleton **`Deer_001_Skeleton`**. Name it **`BS_Deer`**.
4. Select the horizontal axis and set:
   - **Name:** `Speed`  · **Minimum Axis Value:** `0`  · **Maximum Axis Value:** `620`
     (620 = the fastest gait in `AnimalConfig`; keeps the run sample reachable.)
5. Drag clips from the asset list onto the graph track at these speeds
   (double-click the sample to type the exact value):
   - `Deer_001_Anim_Deer_001_idle` at **0**
   - `Deer_001_Anim_Deer_001_walk` at **200** (matches `Walk` gait speed)
   - `Deer_001_Anim_Deer_001_run`  at **520** (matches the reindeer dash)
6. Set **Target Weight Interpolation → Weight Speed ≈ 4–6** to stop pops between poses.
7. Save.

### Step 2 — Animation Blueprint

1. Right-click in `Content/Animals/Deer/` → **Animation → Animation Blueprint**
   (*Animação → Blueprint de Animação*).
2. Pick skeleton **`Deer_001_Skeleton`**. Name it **`ABP_Deer`** (exactly — the code
   looks for `ABP_Deer_C`).
3. **AnimGraph** tab:
   - Drag **`BS_Deer`** into the graph → it becomes a *Blendspace Player*.
   - Connect its **Pose** output to **Output Pose**.
   - Right-click the blendspace **Speed** input pin → **Promote to Variable**. Name it
     `Speed`.
4. **Event Graph** tab (on **Event Blueprint Update Animation** /
   *Evento Atualizar Animação do Blueprint*):
   - **Try Get Pawn Owner** → (out) **Is Valid?** guard.
   - **Get Velocity** → **Vector Length** (*Comprimento do Vetor*).
   - Set variable **`Speed`** = that length.
5. **Compile** (*Compilar*) → **Save**.

### Step 3 — verify

- Play the map. The deer should **idle when stopped, walk when cruising, run when
  fleeing** — driven automatically by ground speed. No code change; the soft path in
  `CreateReindeerConfig` already points here.
- Console log: `AnimalCharacter` logs when it resolves the visual mesh; if the AnimBP
  didn't load you'll see the mesh appear but stay in bind pose (check the asset name is
  exactly `ABP_Deer`).

### Step 4 — repeat

- **Tiger:** skeleton `Tiger_001_Skeleton`, name **`BS_Tiger`** / **`ABP_Tiger`**,
  clips `Tiger_001_Anim_Tiger_001_idle / _walk / _run`. Max axis `980`
  (samples: idle 0, walk 180, run 700).
- **Wolf:** skeleton `low_poly_wolf_Skeleton`, name **`BS_Wolf`** / **`ABP_Wolf`**.
  The wolf has no "walk" clip — use `idle` at 0 and `running` at ~450. Max axis `850`.
  (`sniffing` / `falling` are for later behavior states, not the speed blend.)

## Gotchas

- **Skeleton mismatch:** each BlendSpace/AnimBP is bound to ONE skeleton. Don't drop a
  deer clip into the tiger blendspace.
- **Wrong facing:** if a mesh runs sideways, adjust `VisualMeshRotation` in that species'
  `Create…Config` (currently `(0, -90, 0)`), not the AnimBP.
- **Feet skating:** speed axis samples should roughly match the clip's authored travel
  speed. If the run clip was authored for a faster stride, nudge the run sample value.

## Later (not now)

- Fold the wolf's `sniffing` clip into a behavior state (the AI already has a scent
  seam) via a small **State Machine** upstream of the BlendSpace.
- Add turn-in-place / lean once the base locomotion reads well.
