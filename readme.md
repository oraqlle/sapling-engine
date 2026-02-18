# Sapling Engine

A small lightweight game engine made directly on top of Vulkan.

## Building

Look at the `docs/building.md` file for instructions on how to build Sapling.

## Roadmap

TODO

## Source Tree

Sapling is composed of various subsystems, each working together to allow your games to
run and enabling you to create your games.

* `core/` - Fundamental comonponents to Sapling Engine
* `docs/` - Documentation on how to use the engine and for the project as a whole
* `drivers/` - Platform compatibility layers and subsystems
* `editor/` - GUI application used to create Sapling games (called gumnut or something seed related?)
  * manipulate model 3D scenes
  * load and preview textures and 3D models
  * create scripts files (C++ or Lua [depending on project direction])
  * generate project
  * preview shaders/materials on 3D objects
  * control project settings
* `entry/` - Infrastructure of creating entry point of games built with Sapling
* `external/` - Third party codebases imported using git submodules
* `scripts/` - Helper scripts for setting up developer environment
* `servers/` - Stateful objects used subsystems interact with to fullfil capabilities

