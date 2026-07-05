# Environment — Reviving the Open Tundra

## The real problem (and why we're NOT building a forest)

The current `Tundra.umap` ground is a single **Megascans static mesh**
(`Gigantic_Tundra_Terrain_viyjcbq / viyjcbq_tier_2`). That's why it feels dead:
- it's a frozen photogrammetry slab — you can't sculpt elevation into it,
- flat lighting, no ground cover, obvious tiling, no life.

Swapping it for a forest would *not* fix this, and would hurt the game:

> **Design ruling.** This is a *persistence hunt* on an *isometric camera*. Persistence
> hunting is an OPEN-COUNTRY mechanic — you win by keeping fleeing prey in view and never
> letting the herd rest (Liebenberg; Kalahari runs; `docs/research/HERD_AND_PREY.md`).
> A dense canopy occludes the top-down camera and breaks the core loop. Caribou and saiga
> are open-tundra/steppe animals. **The biome was right; the execution was dead.**

Goal: keep the open tundra, but make it a **living, sculpted Landscape** with ground
scatter and real light. Readability beats realism (art direction, `CLAUDE.md`).

## Assets already in the project

- Tundra ground textures: `Content/Fab/Megascans/3D/Gigantic_Tundra_Terrain_viyjcbq/.../Textures/`
  → `T_viyjcbq_2K_B` (base color), `_N` (normal), `_ORM` (occlusion/rough/metal).
  Reuse these as the Landscape base layer — same look, now paintable and tiling-controlled.
- Material instance `MI_viyjcbq` — reference for the base tundra look.

Free additions to grab from **Fab / Quixel** (Bridge inside UE): boreal/tundra ground
scatter — grass tufts, lichen, low shrubs, small rocks/boulders, and *low* dwarf
birch/willow (knee-height — cover and landmarks, never canopy).

---

## STAGE 1 — Smallest pass that already feels alive (do this first)

Target: replace the dead slab with a sculpted, lit, lightly-scattered Landscape. A few
hours, no PCG needed. Validate the *feel* before investing further.

### 1.1 Create the Landscape

1. In `Tundra.umap`, **hide/delete** the `viyjcbq_tier_2` static-mesh ground actor
   (keep it in the Content Browser as reference — don't delete the asset).
2. Toolbar mode dropdown → **Landscape** (*Paisagem*) → **Manage → Create**.
3. Section size default; set overall size to cover your play area (e.g. 1009×1009 or
   scale a 63×63-quads landscape). Create.

### 1.2 Sculpt for the hunt, not for spectacle

Open tundra is *mostly* flat — but flat-flat is boring and reads as dead. Add:
- gentle rolling undulation across the whole sheet (**Sculpt** brush, very low strength,
  huge radius),
- a few **low ridges and shallow dips** — "dead ground" a fleeing animal can crest and
  briefly vanish behind. This is *tactical terrain*: it creates hunt moments without
  blocking the camera.
- Keep slopes gentle so the isometric read stays clean and the herd stays visible.

### 1.3 Landscape material (layered)

1. Create a **Landscape Material** (`M_Tundra`) with a **Landscape Layer Blend** node.
2. Layers (each = the Megascans `B/N/ORM` set, tiled):
   - `Base_Moss` → the `viyjcbq` tundra textures (the ground you already have),
   - `Rock_Gravel` → a Megascans rock/gravel set (for ridges/exposed ground),
   - `Snow_Patch` → optional pale layer for hollows/north faces.
3. Assign to the Landscape. In **Paint** (*Pintura*) mode, paint gravel onto ridges and
   snow into dips so the ground isn't one flat tone. **Value contrast is what makes
   animals pop** against it from above.

### 1.4 Lighting & atmosphere (huge feel-per-minute)

- **Directional Light**: low sun angle (~10–20° above horizon) → long raking shadows,
  the arctic/steppe read. Slightly cool-warm temperature.
- **Sky Atmosphere** + **SkyLight** (recapture after placing).
- **Exponential Height Fog**, subtle — gives depth and a horizon so the world doesn't
  look like a floating sheet. A touch of **Volumetric Fog** for morning haze is cheap
  and sells atmosphere.
- A mild post-process **color grade** (cool shadows, desaturated) unifies everything.

### 1.5 A little scatter (manual, to validate)

Use the **Foliage** (*Folhagem*) tool for a first pass — hand-paint:
- grass tufts + lichen across the sheet (medium density),
- rocks/boulders clustered on ridges,
- sparse dwarf-shrub clumps as landmarks.

> **Validate here.** Play the map from the game camera. Does it feel alive? Can you still
> read the herd clearly at hunt distance? If yes, Stage 1 was enough for now — move on and
> come back for Stage 2 when you want scale.

---

## STAGE 2 — Scale it with PCG (when Stage 1 feels right)

Hand-painting 10 km² of tundra doesn't scale. UE5.6's **PCG (Procedural Content
Generation)** does — this is the transferable technique from the 80.lv forest article,
dialed DOWN for open, readable tundra.

1. Enable the **PCG** plugin (and **PCG Biome Core / Biome Sample** if you want the ready
   biome graphs).
2. Create a **PCG Graph**: Surface Sampler on the Landscape → density filters by slope
   and layer weight (grass on flats, rock on steep, nothing on snow) → Static Mesh Spawner
   for each scatter type.
3. Drop a **PCG Volume** over the play area. Tune **density low** — tundra is sparse.
   Keep dwarf shrubs knee-height; no tall occluders.
4. Rule of thumb for readability: if a scatter mesh is taller than the player animal's
   shoulder, it's too tall for this camera. Cut it or shrink it.

## STAGE 3 — Polish (optional, later)

- Wind on grass/shrubs (subtle — motion sells "alive").
- Runtime Virtual Texturing so scatter blends into the ground (kills the "stickers on a
  floor" look).
- Seasonal tint variants (summer green-brown ↔ winter pale) if the design wants time.
- Distance-field / Nanite on boulders; HLODs for the far field.

## Readability guardrails (recurring checklist)

- [ ] Herd clearly visible from the game camera at hunt distance.
- [ ] No scatter taller than an animal's shoulder near play space.
- [ ] Ground has value contrast so animals don't camouflage into it.
- [ ] Elevation creates *hunt moments* (dead ground, cresting) without hiding prey.
- [ ] Fog/atmosphere gives depth but never greys out the readable midground.
