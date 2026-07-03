# Asset Bible — The First Life

> Acquisition plan for all bought/free assets (D10: buy what can be bought; spec: art is
> acquired, never modeled). Researched 2026-07-03 against Fab, Sketchfab, Sonniss, itch.io,
> Google Fonts. **Caveat:** Fab prices move with sales and some listing pages resist
> verification — prices marked *unverified* are last-seen, not gospel. Re-check the
> listing's animation list and reviews before every purchase. All Fab acquisitions
> (free or paid) are licensed forever under the Fab Standard License.

**Two tiers everywhere:** *Placeholder* = free, good enough to validate gameplay now.
*Replacement* = paid, bought only after the slice that needs it is validated fun.

**Standing rules**
1. Human animations live on the **Epic/UE5 Manny skeleton forever**. Buy human meshes
   rigged to it; never buy a custom-rigged human.
2. Every animal seller ships a custom quadruped skeleton. Budget one IK-Rig/retarget
   pipeline **per species**, not shared anims.
3. Extinct animals have no recordings and few models: mammoth audio = pitched-down
   elephant; smilodon audio = layered lion/tiger. Plan sound-design time, not money.

---

## Characters

### Homo sapiens (early human)

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| Game Animation Sample (Epic) | [fab.com](https://www.fab.com/listings/880e319a-a59e-4ed2-b268-b32dac7fa016) | **Free** | 500+ AAA locomotion (walk/jog/sprint/traversal, motion matching) | UE5 Manny native | AAA mocap | **Placeholder + permanent anim foundation** — maps 1:1 onto our Walk/Jog/Sprint gaits |
| Modular Barbarian / Tribe pack | [fab.com](https://www.fab.com/listings/b98d19d9-c0d5-48eb-af19-46642489f0de) | ~$30–60 *unverified* | Mesh pack (few/none) | UE Mannequin / MetaHuman variant | Realistic-stylized tribal — closest "early sapiens" body kit found | **Replacement mesh** — retargets Manny anims trivially |
| Spear MocapAnimPack (MoCap Online) | [fab.com](https://www.fab.com/listings/4830cec0-ddba-4f34-a493-6e6f9594cbc1) | ~$50–100 *unverified* | 170 spear anims (thrust, throw, spear locomotion) | UE5 Mannequin, no retarget | AAA mocap | **Buy when the hunt slice needs a spear** — persistence hunting's core verb |

### Wolf

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| ANIMAL VARIETY PACK | [fab.com](https://www.fab.com/listings/2dd7964c-a601-4264-a53d-465dcae1644c) | **Free** (Epic-sponsored) | Wolf: 26 (locomotion/attack/hit/death) + AnimBP | Custom quadruped | Solid mid-realistic, 10K tris | **Placeholder** — also covers deer; the obvious first upgrade from grey-box |
| Animalia – Gray Wolf (GiM) | [fab.com](https://www.fab.com/listings/82365785-fa5a-43ee-8c30-0f70607ee22a) | ~$90–150 *unverified* | 60 fps full behavior set, 4K, gFur | GiM custom + AnimBP | Best-in-class wildlife | **Replacement** — worth it on any species the player can *be* |
| Realistic Wolf (+cub) | [fab.com](https://www.fab.com/listings/98fd8cf3-b542-4f5f-a131-42663de9e788) | ~$40–80 *unverified* | 100+ anims, root motion + in-place | Custom, no controller BPs | Hair-card fur | Mid-price fallback if Animalia stays pricey |

### Smilodon

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| Smilodon Populator (Kyan0s) | [fab.com](https://www.fab.com/listings/77cb6438-44b9-4249-98f4-c08343687191) | ~$30–80 *unverified* | Animated; set list unverified; 90.7K tris (needs LODs/Nanite) | Blender rig — retarget work | Realistic, respected paleo artist | **Replacement** — only credible dedicated Smilodon; verify anim list first |
| AnimX: Advanced Cats | [fab.com](https://www.fab.com/listings/7b57ab44-9bc9-4564-879d-f99f42ddff67) | *unverified* | Full big-cat controller (UE5.7-updated) | AnimX controller | Realistic | Fallback: generic big cat + saber kitbash if Kyan0s anims are thin |
| — placeholder | — | free | — | — | — | Scale + tint the free wolf as grey-box stand-in; no free smilodon exists |

### Mammoth

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| The Mammoth | [fab.com](https://www.fab.com/listings/bf50cbfa-2c13-4250-96a8-93ca84a1dad1) | ~$60–100 *unverified* | 14 in-place (attack ×2, eat, idle, run, stand, trumpet, turns, walk) + AnimBP | Custom, 5 LODs, groom fur, 4K | Realistic, 4.8★ | **Replacement** — purpose-built; note: in-place only, no root motion |
| African elephant (low poly) | [fab.com](https://www.fab.com/listings/ebf080aa-89af-44e5-87cf-c6dfef494f03) | cheap *unverified* | 21 anims, root + in-place | Custom, 4.5K tris | Low-poly | **Placeholder** elephant base |

### Deer

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| ANIMAL VARIETY PACK (Stag + Doe) | (same free pack as wolf) | **Free** | 20 each + AnimBP | Custom | Good | **Placeholder** — genuinely enough for the chase prototype |
| Animalia – Red Deer pack | [unrealengine.com](https://www.unrealengine.com/marketplace/en-US/product/animalia-red-deer-pack) | ~$90–150 *unverified* | 60 fps full set, 4K | GiM custom | Best realism | **Replacement** — red deer is period-correct Pleistocene Eurasia; pack = stag+hind for herd readability |

### Horse (wild / Przewalski)

| Asset | Source | Price | Animations | Rig | Quality | Recommendation |
|---|---|---|---|---|---|---|
| Horse Animset (MalberS) | [fab.com](https://www.fab.com/listings/e0fc6520-8c9d-425d-9d25-3b6dc9ed56f8) | **from $74.99** (verified) | 200+ AAA (walk/trot/canter/gallop/sprint, swim, jump, attack, death) | Custom horse rig; riding BPs absent on UE | Best gaits on the market | **Replacement** — horses are prey here, so missing riding BPs cost nothing; retexture dun + upright mane for Przewalski |
| Horse Herd | [fab.com](https://www.fab.com/listings/6b3bb5d4-5057-4e77-bcc9-eaf455171959) | *unverified* | Herd locomotion, root + in-place | Custom, 24K tris | Mid | **Placeholder** if cheap; else grey-box |

---

## Environment

| Category | Placeholder (free) | Replacement (paid) |
|---|---|---|
| **Rocks** | Megascans free tier on Fab (800+ legacy assets + rotating free drops — claim monthly: [fabfreecontent](https://www.unrealengine.com/fabfreecontent)) | **None needed.** Buy a specific Megascans collection only if a biome gap appears; no third-party rock pack beats this |
| **Grass** | Free Megascans grass patches + UE landscape grass types | Interactive Open World Foliage ([fab](https://www.fab.com/listings/eba98f8c-d99a-4f60-8495-5db0e62666ff), ~$20–40 *unverified*) — 1-tri-per-blade mesh grass; predictable overdraw beats Nanite-grass claims on mid hardware |
| **Trees** (steppe/taiga) | Free Megascans/Megaplants conifers (rotating "European Broadleaf Forest"-style drops through 2026) | Taiga Biome Environment ([fab](https://www.fab.com/listings/513ac474-f1f6-4e53-b9f9-65c6a1a5c0fd), **from $34.99**, UE 5.0–5.6 confirmed, needs Virtual Textures) — cheapest exact-biome match |
| **Snow** | Landscape layer blend (an afternoon of work) | Winter Procedural Landscape ([fab](https://www.fab.com/listings/891aba4e-8ec1-4215-8f62-b1cc34d38121), *unverified*) — auto material + snow/mist Niagara + spawners. **Skip ClimaX-style full weather systems** unless dynamic weather becomes a validated pillar (scope trap) |

## Audio

| Category | Placeholder (free) | Replacement (paid) |
|---|---|---|
| **Footsteps** (barefoot, dirt/grass/snow) | Sonniss GDC 2026 bundle (7.47 GB, royalty-free, no attribution: [gdc.sonniss.com](https://gdc.sonniss.com/)) + freesound CC0 | Essential Footsteps Pack ([fab](https://www.fab.com/listings/a40d5345-eaf0-4bdd-a36d-959ce6b38a70), *unverified*) — 3,263 sounds, **explicit barefoot** tier; fallback: Footsteps Integral (4,700+, barefoot *and* snow confirmed) |
| **Animal sounds** | GDC bundle archives (~160 GB across years; wolf/horse/elephant present) | Sonniss "Animals" (200 tracks: wildcats, elephants, wolves) + Sonniss deer-rut library (267 stag roars — exceptional for deer AI barks). Mammoth/smilodon **must** be designed: pitched elephant / lion+tiger layers |
| **Wind** (steppe/tundra) | freesound CC0 loops + GDC bundle | Wind Sound Pack ([marketplace](https://www.unrealengine.com/marketplace/en-US/product/wind-sound-pack), *unverified*) — 90 SFX, breeze→storm range maps to weather/stamina states |
| **Water** (rivers/streams) | GDC bundle / freesound | Procedural River Sound ([fab](https://www.fab.com/listings/aa57cc4d-23d6-4906-97e4-65f5b5de6ad2), 5.0★, *unverified*) — 5 water types, spline-friendly emitter logic, not just WAVs |

## UI

| Category | Placeholder → likely final (free) | Paid upgrade (only at polish/branding stage) |
|---|---|---|
| **Icons** | [game-icons.net](https://game-icons.net/) — 4,180+ SVGs, CC BY 3.0 (credit authors), deep survival coverage (meat, paw prints, campfire, spear, stamina); restyle SVGs to our look. Kenney (CC0) for input prompts | Commission custom icons at polish; no paid pack justified before then |
| **Fonts** | Caesar Dressing (incised/primal display, caps only) + Inter or Alegreya Sans (body/HUD) — both Google Fonts, OFL, safe to ship | Wild Flint "primal stone-age" display (Envato Elements, ~$16.50/mo sub, *unverified*) — check game-embedding terms first |

---

## Budget picture

- Verified anchors: MalberS Horse **$74.99**, Taiga Biome **$34.99+**. Animalia singles ~$90–150 each (*unverified*).
- Full replacement tier lands **≈ $400–700**, ~70% of it in the five animal characters.
- The free tier ships a complete playable slice at **$0** — the only cost is audio curation time.
- Known gaps: **no Animalia horse or mammoth exists** (use MalberS + "The Mammoth"); un-migrated legacy Marketplace listings are inaccessible unless already vaulted — never plan around them.

## Acquisition order (gated by slices, per D9/D10)

1. **Now (free, ~1 h):** Game Animation Sample + ANIMAL VARIETY PACK + monthly Megascans free drops + GDC 2026 audio bundle + game-icons.net + fonts.
2. **Slice 2 (living herd):** free deer already covered; add Taiga Biome if grey-box hurts readability.
3. **Post-validation of the hunt:** Spear MocapAnimPack, Animalia Red Deer, human mesh (Barbarian/Tribe).
4. **New-species slices, each gated on the previous being fun:** Animalia Wolf → MalberS Horse → The Mammoth → Kyan0s Smilodon.
