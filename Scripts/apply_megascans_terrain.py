"""Applies the downloaded Megascans tundra terrain to /Game/Maps/GreyBox.

Run inside UnrealEditor after the Fab asset has been added to the project:
Tools > Execute Python Script > Scripts/apply_megascans_terrain.py
"""

import os
import sys

SCRIPT_DIR = os.path.dirname(__file__)
if SCRIPT_DIR not in sys.path:
    sys.path.insert(0, SCRIPT_DIR)

import build_greybox_map

build_greybox_map.build_map()
