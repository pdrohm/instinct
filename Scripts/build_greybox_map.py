"""Builds the Slice 1 grey-box map and the playable species' data asset.

Run inside UnrealEditor: Tools > Execute Python Script (or `py` console command).
Idempotent: skips anything that already exists. Produces:
  /Game/Maps/GreyBox    - floor, boundary walls, LOS blockers, PlayerStart, lights
  /Game/Agents/DA_Human - UAnimalConfig instance (edit this to tune feel, no recompile)
"""

import unreal

LEVEL_PATH = "/Game/Maps/GreyBox"
HUMAN_CONFIG_PATH = "/Game/Agents/DA_Human"
CUBE_MESH_PATH = "/Engine/BasicShapes/Cube.Cube"

# Map is 100m x 100m; the cube primitive is 100uu (1m) per side before scaling.
FLOOR_SCALE = unreal.Vector(100.0, 100.0, 1.0)
WALL_HEIGHT_Z = 150.0
HALF_MAP = 5000.0


def spawn_box(actor_subsystem, cube_mesh, label, location, scale):
    actor = actor_subsystem.spawn_actor_from_class(
        unreal.StaticMeshActor, location, unreal.Rotator(0.0, 0.0, 0.0))
    actor.set_actor_label(label)
    actor.static_mesh_component.set_static_mesh(cube_mesh)
    actor.set_actor_scale3d(scale)
    return actor


def build_map():
    level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(LEVEL_PATH):
        unreal.log("GreyBox map already exists - loading it, not rebuilding.")
        level_subsystem.load_level(LEVEL_PATH)
        return

    level_subsystem.new_level(LEVEL_PATH)
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    cube = unreal.EditorAssetLibrary.load_asset(CUBE_MESH_PATH)

    # Ground and hard boundaries (spec: hard boundaries, no navigation beyond).
    spawn_box(actors, cube, "Floor", unreal.Vector(0, 0, -50), FLOOR_SCALE)
    spawn_box(actors, cube, "Wall_N", unreal.Vector(0, HALF_MAP, WALL_HEIGHT_Z), unreal.Vector(100, 0.5, 4))
    spawn_box(actors, cube, "Wall_S", unreal.Vector(0, -HALF_MAP, WALL_HEIGHT_Z), unreal.Vector(100, 0.5, 4))
    spawn_box(actors, cube, "Wall_E", unreal.Vector(HALF_MAP, 0, WALL_HEIGHT_Z), unreal.Vector(0.5, 100, 4))
    spawn_box(actors, cube, "Wall_W", unreal.Vector(-HALF_MAP, 0, WALL_HEIGHT_Z), unreal.Vector(0.5, 100, 4))

    # A few line-of-sight breakers (spec: rocks, a rise, sparse cover).
    spawn_box(actors, cube, "Rock_A", unreal.Vector(1500, 900, 150), unreal.Vector(6, 4, 4))
    spawn_box(actors, cube, "Rock_B", unreal.Vector(-1200, -2200, 120), unreal.Vector(4, 5, 3))
    spawn_box(actors, cube, "Rock_C", unreal.Vector(2800, -1500, 100), unreal.Vector(3, 3, 2.5))
    spawn_box(actors, cube, "Rise", unreal.Vector(-2400, 1800, 40), unreal.Vector(18, 12, 1.6))

    player_start = actors.spawn_actor_from_class(
        unreal.PlayerStart, unreal.Vector(0, 0, 120), unreal.Rotator(0.0, 0.0, 0.0))
    player_start.set_actor_label("PlayerStart")

    # Movable lights + real-time skylight: no lighting build needed, ever.
    sun = actors.spawn_actor_from_class(
        unreal.DirectionalLight, unreal.Vector(0, 0, 2000), unreal.Rotator(-50.0, 30.0, 0.0))
    sun.set_actor_label("Sun")
    sun.root_component.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
    # Drive the SkyAtmosphere (else the sky renders black and the real-time SkyLight,
    # capturing that black sky, provides no ambient fill — the whole scene goes dark in Lit).
    sun.light_component.set_editor_property("atmosphere_sun_light", True)
    sun.light_component.set_editor_property("intensity", 10.0)

    sky_light = actors.spawn_actor_from_class(
        unreal.SkyLight, unreal.Vector(0, 0, 2000), unreal.Rotator(0.0, 0.0, 0.0))
    sky_light.set_actor_label("SkyLight")
    sky_light.root_component.set_editor_property("mobility", unreal.ComponentMobility.MOVABLE)
    sky_light.light_component.set_editor_property("real_time_capture", True)

    atmosphere = actors.spawn_actor_from_class(
        unreal.SkyAtmosphere, unreal.Vector(0, 0, 0), unreal.Rotator(0.0, 0.0, 0.0))
    atmosphere.set_actor_label("SkyAtmosphere")

    level_subsystem.save_current_level()
    unreal.log("GreyBox map created and saved.")


def build_human_config():
    if unreal.EditorAssetLibrary.does_asset_exist(HUMAN_CONFIG_PATH):
        unreal.log("DA_Human already exists - leaving your tuning untouched.")
        return

    factory = unreal.DataAssetFactory()
    factory.set_editor_property("data_asset_class", unreal.AnimalConfig.static_class())
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    asset_tools.create_asset("DA_Human", "/Game/Agents", unreal.AnimalConfig.static_class(), factory)
    unreal.EditorAssetLibrary.save_asset(HUMAN_CONFIG_PATH)
    unreal.log("DA_Human created with class-default tuning (early Homo sapiens endurance profile). "
               "Edit it to iterate on feel.")


build_map()
build_human_config()
