# Sapling Engine

A small lightweight game engine made directly on top of Vulkan.

## Building

TODO

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
* `servers/` - Stateful objects used subsystems interact with to fullfil capabilities

> Note: Initial architecture inspired from Godot's
> * <https://docs.godotengine.org/en/stable/engine_details/architecture/godot_architecture_diagram.html>
> * <https://godotengine.org/article/why-does-godot-use-servers-and-rids/>

