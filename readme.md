# Sapling Engine

A small lightweight game engine made directly on top of Vulkan.

## Documentation

* **Licensing:** GNU Lesser General Public License v2.1 [`LICENSE`](LICENSE)
* **Building:** [`docs/building.md`](docs/building.md)
* **Roadmap:** [`docs/building.md`](docs/roadmap.md)
* **Contributing:** [`docs/contributing.md`](docs/contributing.md)
* **Engine Architecture:** [`docs/architecture.md`](docs/architecture.md)

## Subsystems

Sapling is composed of various subsystems, each working together to allow your games to
run and enabling you to create your games.

* `core/` - Fundamental components to Sapling Engine
* `docs/` - Documentation on how to use the engine and for the project as a whole
* `drivers/` - Platform compatibility layers and subsystems
* `editor/` - GUI application used to create Sapling games (called gumnut or something seed related?)
  * manipulate model 3D scenes
  * load and preview textures and 3D models
  * create scripts files (C++ or Lua [depending on project direction])
  * generate project
  * preview shaders/materials on 3D objects
  * control project settings
* `entrypoint/` - Infrastructure of creating entry point of games built with Sapling
* `servers/` - Stateful objects used subsystems interact with to fulfil capabilities
