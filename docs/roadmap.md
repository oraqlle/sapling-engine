# Roadmap

* Version: 1.0 :: (15/03/2026)

 ---

* Project Scoping
  * Initial project outline
  * Establish project goal(s) and outcomes
  * Obtain Academic Supervisor(s) for project
    * > Dr Yongqiang Tian (primary supervisor - Department of Software Systems and
        Cybersecurity)
    * > Josh Olsen (secondary supervisor - Department of Games and Immersive Media)
  * Establish possible research questions for project:
    * > Performance of Entity Component System vs Data Oriented Programming in Game
      > Engine Architecture?
    * > How Lean can a Game Engine be?
  * What tools / platforms to use?
    * > OpenGL vs Vulkan
      * > Consulted 2nd supervisor, suggested Vulkan as it is more modern
    * > Use a thirdparty Physics Engine?
    * > Use a thirdparty Entity Component System?

* Pre-Reading & Spikes
  * Graphics API
      * OpenGL Reading
        * Khronos Group Wiki
          * > abc
        * Learn OpenGL <learnopengl.com>
          * > abc
      * Vulkan  Reading
        * Vulkan Website
  * Physics Engine
    * Bullet
    * Jolt
    * ReactPhysics3D
  * Entity Component System (ECS)
    * entt
    * "in-house" ie. native to engine

* Planning
  * Engine Architecture
  * Draft Roadmap
  * Decide what demo game should be made

* Engine Development - Phase One
  * Core Subsystem
    * Entity Component System
    * Resource Handlers
    * Basic Renderer
    * Event System
  * Build System
    * Modular subsystems encoded in build process
    * Cross platform installation from source rules
  * Entry point and engine bootstrapping

* Engine Development - Phase Two
  * Notion of a Camera
  * Lighting, materials and shader pipeline
  * Loading Textures
  * Loading Meshes

* Engine Development - Phase Three
  * Additional Server Subsystems
    * Audio
    * Physics
    * Animation
  * Engine Editor
    * Scene manipulation / level editing
    * Material Editor
    * Loading and saving configurations
  * Develop demo game showcasing engine features
