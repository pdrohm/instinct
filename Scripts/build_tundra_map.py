"""Builds the real tundra play map from the downloaded Megascans terrain.

Run inside UnrealEditor, or via UnrealEditor-Cmd -ExecutePythonScript.
Produces:
  /Game/Maps/Tundra - Megascans terrain, PlayerStart, sun, skylight, atmosphere
"""

import unreal

LEVEL_PATH = "/Game/Maps/Tundra"
TERRAIN_FOLDER = "/Game/Fab/Megascans/3D/Gigantic_Tundra_Terrain_viyjcbq/Medium"

TERRAIN_LABEL = "Tundra_Terrain"
PLAYER_START_LABEL = "PlayerStart"
SUN_LABEL = "Sun"
SKY_LIGHT_LABEL = "SkyLight"
SKY_ATMOSPHERE_LABEL = "SkyAtmosphere"


def find_actor_by_label(actor_subsystem, label):
    for actor in actor_subsystem.get_all_level_actors():
        if actor.get_actor_label() == label:
            return actor
    return None


def find_or_spawn_actor(actor_subsystem, actor_class, label, location, rotation):
    actor = find_actor_by_label(actor_subsystem, label)
    if not actor:
        actor = actor_subsystem.spawn_actor_from_class(actor_class, location, rotation)
        actor.set_actor_label(label)
    return actor


def set_editor_property_safe(obj, property_name, value):
    try:
        obj.set_editor_property(property_name, value)
    except Exception as exc:
        unreal.log_warning(f"Could not set {property_name} on {obj}: {exc}")


def find_first_static_mesh(folder_path):
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        raise RuntimeError(f"Missing terrain folder: {folder_path}")

    for asset_path in unreal.EditorAssetLibrary.list_assets(
            folder_path, recursive=True, include_folder=False):
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if isinstance(asset, unreal.StaticMesh):
            return asset

    raise RuntimeError(f"No StaticMesh found under terrain folder: {folder_path}")


def apply_terrain(actor_subsystem):
    terrain_mesh = find_first_static_mesh(TERRAIN_FOLDER)
    terrain = find_or_spawn_actor(
        actor_subsystem,
        unreal.StaticMeshActor,
        TERRAIN_LABEL,
        unreal.Vector(0, 0, 0),
        unreal.Rotator(0, 0, 0))
    terrain.static_mesh_component.set_static_mesh(terrain_mesh)
    terrain.set_actor_location(unreal.Vector(0, 0, 0), False, False)
    terrain.set_actor_rotation(unreal.Rotator(0, 0, 0), False)
    terrain.set_actor_scale3d(unreal.Vector(1, 1, 1))
    try:
        terrain.static_mesh_component.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
    except Exception as exc:
        unreal.log_warning(f"Could not enable terrain collision: {exc}")
    unreal.log(f"Tundra terrain set to {terrain_mesh.get_path_name()}")


def apply_player_start(actor_subsystem):
    start = find_or_spawn_actor(
        actor_subsystem,
        unreal.PlayerStart,
        PLAYER_START_LABEL,
        unreal.Vector(0, 0, 1200),
        unreal.Rotator(0, 0, 0))
    start.set_actor_location(unreal.Vector(0, 0, 1200), False, False)


def apply_lighting(actor_subsystem):
    sun = find_or_spawn_actor(
        actor_subsystem,
        unreal.DirectionalLight,
        SUN_LABEL,
        unreal.Vector(0, 0, 5000),
        unreal.Rotator(-42, -28, 0))
    sun.set_actor_rotation(unreal.Rotator(-42, -28, 0), False)
    set_editor_property_safe(sun.root_component, "mobility", unreal.ComponentMobility.MOVABLE)
    set_editor_property_safe(sun.light_component, "atmosphere_sun_light", True)
    set_editor_property_safe(sun.light_component, "cast_shadows", True)
    set_editor_property_safe(sun.light_component, "intensity", 75000.0)
    set_editor_property_safe(sun.light_component, "light_color", unreal.Color(255, 235, 199, 255))

    sky = find_or_spawn_actor(
        actor_subsystem,
        unreal.SkyLight,
        SKY_LIGHT_LABEL,
        unreal.Vector(0, 0, 3000),
        unreal.Rotator(0, 0, 0))
    set_editor_property_safe(sky.root_component, "mobility", unreal.ComponentMobility.MOVABLE)
    set_editor_property_safe(sky.light_component, "real_time_capture", True)
    set_editor_property_safe(sky.light_component, "intensity", 1.25)

    find_or_spawn_actor(
        actor_subsystem,
        unreal.SkyAtmosphere,
        SKY_ATMOSPHERE_LABEL,
        unreal.Vector(0, 0, 0),
        unreal.Rotator(0, 0, 0))


def build_map():
    level_subsystem = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if unreal.EditorAssetLibrary.does_asset_exist(LEVEL_PATH):
        level_subsystem.load_level(LEVEL_PATH)
    else:
        level_subsystem.new_level(LEVEL_PATH)

    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    apply_terrain(actors)
    apply_player_start(actors)
    apply_lighting(actors)
    level_subsystem.save_current_level()
    unreal.log(f"Tundra map saved: {LEVEL_PATH}")


build_map()
