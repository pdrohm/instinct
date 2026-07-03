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
MEGASCANS_TERRAIN_FOLDER = "/Game/Fab/Megascans/3D/Gigantic_Tundra_Terrain_viyjcbq/Medium"
TERRAIN_ACTOR_LABEL = "Megascans_Tundra_Terrain"
SUN_LABEL = "Sun"
SKY_LIGHT_LABEL = "SkyLight"
SKY_ATMOSPHERE_LABEL = "SkyAtmosphere"

# Map is 100m x 100m; the cube primitive is 100uu (1m) per side before scaling.
FLOOR_SCALE = unreal.Vector(100.0, 100.0, 1.0)
WALL_HEIGHT_Z = 150.0
HALF_MAP = 5000.0


def find_actor_by_label(actor_subsystem, label):
    for actor in actor_subsystem.get_all_level_actors():
        if actor.get_actor_label() == label:
            return actor
    return None


def set_editor_property_safe(obj, property_name, value):
    try:
        obj.set_editor_property(property_name, value)
    except Exception as exc:
        unreal.log_warning(f"Could not set {property_name} on {obj}: {exc}")


def find_or_spawn_actor(actor_subsystem, actor_class, label, location, rotation):
    actor = find_actor_by_label(actor_subsystem, label)
    if not actor:
        actor = actor_subsystem.spawn_actor_from_class(actor_class, location, rotation)
        actor.set_actor_label(label)
    return actor


def find_first_static_mesh(folder_path):
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        unreal.log_warning(
            f"Megascans terrain folder not found: {folder_path}. "
            "Add the Fab asset to the project first, then rerun this script.")
        return None

    for asset_path in unreal.EditorAssetLibrary.list_assets(
            folder_path, recursive=True, include_folder=False):
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if isinstance(asset, unreal.StaticMesh):
            return asset

    unreal.log_warning(f"No StaticMesh found under Megascans terrain folder: {folder_path}")
    return None


def apply_megascans_terrain():
    terrain_mesh = find_first_static_mesh(MEGASCANS_TERRAIN_FOLDER)
    if not terrain_mesh:
        return False

    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    terrain_actor = find_actor_by_label(actors, TERRAIN_ACTOR_LABEL)
    if not terrain_actor:
        terrain_actor = find_actor_by_label(actors, "Floor")

    if not terrain_actor:
        terrain_actor = actors.spawn_actor_from_class(
            unreal.StaticMeshActor, unreal.Vector(0, 0, 0), unreal.Rotator(0.0, 0.0, 0.0))

    terrain_actor.set_actor_label(TERRAIN_ACTOR_LABEL)
    terrain_actor.static_mesh_component.set_static_mesh(terrain_mesh)
    terrain_actor.set_actor_location(unreal.Vector(0, 0, 0), False, False)
    terrain_actor.set_actor_rotation(unreal.Rotator(0.0, 0.0, 0.0), False)
    terrain_actor.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
    unreal.log(f"GreyBox map terrain wired to {terrain_mesh.get_path_name()}")
    return True


def apply_lighting():
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    sun = find_or_spawn_actor(
        actors,
        unreal.DirectionalLight,
        SUN_LABEL,
        unreal.Vector(0, 0, 5000),
        unreal.Rotator(-42.0, -28.0, 0.0))
    sun.set_actor_rotation(unreal.Rotator(-42.0, -28.0, 0.0), False)
    set_editor_property_safe(sun.root_component, "mobility", unreal.ComponentMobility.MOVABLE)
    set_editor_property_safe(sun.light_component, "atmosphere_sun_light", True)
    set_editor_property_safe(sun.light_component, "cast_shadows", True)
    set_editor_property_safe(sun.light_component, "intensity", 75000.0)
    set_editor_property_safe(sun.light_component, "light_color", unreal.LinearColor(1.0, 0.92, 0.78, 1.0))

    sky_light = find_or_spawn_actor(
        actors,
        unreal.SkyLight,
        SKY_LIGHT_LABEL,
        unreal.Vector(0, 0, 3000),
        unreal.Rotator(0.0, 0.0, 0.0))
    set_editor_property_safe(sky_light.root_component, "mobility", unreal.ComponentMobility.MOVABLE)
    set_editor_property_safe(sky_light.light_component, "real_time_capture", True)
    set_editor_property_safe(sky_light.light_component, "intensity", 1.25)

    find_or_spawn_actor(
        actors,
        unreal.SkyAtmosphere,
        SKY_ATMOSPHERE_LABEL,
        unreal.Vector(0, 0, 0),
        unreal.Rotator(0.0, 0.0, 0.0))

    unreal.log("GreyBox lighting refreshed: movable sun, skylight, and sky atmosphere.")


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
        unreal.log("GreyBox map already exists - loading it, then applying terrain wiring.")
        level_subsystem.load_level(LEVEL_PATH)
        apply_megascans_terrain()
        apply_lighting()
        level_subsystem.save_current_level()
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

    apply_megascans_terrain()
    apply_lighting()

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
