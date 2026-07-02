# Setup — Slice 1: Wolf + Stamina

The project targets **UE 5.6** (not yet installed on this machine when the slice was authored).

## One-time setup

1. Install **Unreal Engine 5.6** via the Epic Games Launcher (Xcode 26 is already present).
2. Double-click `FirstLife.uproject`. Accept the prompt to build the missing `FirstLife` module
   (or right-click the `.uproject` → *Generate Xcode Project* and build `FirstLifeEditor` there).
3. In the editor: **Tools → Execute Python Script… → `Scripts/build_greybox_map.py`**.
   This creates the `GreyBox` map (floor, walls, rocks, lights, PlayerStart) and the
   `DA_Wolf` config asset. It is idempotent — safe to re-run.
4. Press **Play**.

## Controls

| Input | Action |
|-------|--------|
| WASD | Move |
| Mouse | Look |
| Left Shift (hold) | Sprint — drains stamina; dies mid-stride at exhaustion |
| P | Toggle possession: hand the wolf to its AI brain / reclaim it (H7 seam) |

## Tuning (no recompile)

Edit `Content/Agents/DA_Wolf` — speeds, drain/regen rates, exhaustion recovery threshold.
The chase/energy rhythm IS the product of this slice; iterate there.

## What this slice validates

- **H1/H2** — does the stamina budget create a movement rhythm and a reason to ration sprint?
- **H3** — does inhabiting the body (camera, momentum, exhaustion) feel like *being* an animal?
- **H7** — possession seam: P proves the wolf is an ordinary agent a human happens to drive.

Out of scope by design: herd, awareness/flee, prey stamina, hunger, scent, day/night,
menus, audio, save, multiplayer, Mass Entity.
