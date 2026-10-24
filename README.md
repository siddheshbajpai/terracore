# TerraCore

**A modular C++ 3D engine built with Vulkan.**

TerraCore is an open-source 3D engine project focused on building a solid foundation for real-time rendering, procedural environments, and game development. The long-term goal is to support both editable voxel worlds and conventional 3D scenes within the same engine.

The project is being developed incrementally, starting with the rendering and platform foundations before moving into terrain generation, scene management, and more advanced engine systems.

TerraCore is an early-stage project. The architecture, feature set, and development priorities will evolve as the implementation takes shape. Features described in the roadmap are planned, not claims about what is already available.

---

## Table of Contents

- [Overview](#overview)
- [Project Goals](#project-goals)
- [Planned Features](#planned-features)
- [Architecture](#architecture)
- [Technology Choices](#technology-choices)
- [Development Roadmap](#development-roadmap)
- [Build and Development](#build-and-development)
- [Design Principles](#design-principles)
- [Testing and Performance](#testing-and-performance)
- [Contributing](#contributing)
- [Licensing](#licensing)
- [Project Status](#project-status)

## Overview

Many 3D projects eventually need the same underlying systems: rendering, asset management, scene organization, resource handling, and tools for inspecting performance. TerraCore is an attempt to build these foundations as a reusable engine rather than tie them to a single game.

Voxel worlds are an important part of the project, but they are not the entire engine. A voxel terrain system and a conventional 3D scene should be able to share rendering, cameras, materials, and other common infrastructure without becoming dependent on one another.

The immediate objective is deliberately modest: get a clean C++ project building reliably, establish a working Vulkan renderer, and expand it through small, verifiable milestones.

The larger vision is to grow that foundation into an engine suitable for experimentation, learning, and eventually the development of complete games.

## Project Goals

### A maintainable engine architecture

Engine systems should have clear responsibilities and interfaces. Rendering, platform integration, scene management, asset loading, and voxel terrain should not be tangled into a single application or a collection of tightly coupled classes.

### A renderer that can grow

Vulkan provides explicit control over GPU resources and rendering work, along with the responsibility of managing those resources correctly. TerraCore will use that control to build a renderer that can evolve from basic mesh rendering toward more advanced real-time graphics.

### First-class voxel terrain

The voxel system should support editable chunk data, procedural terrain generation, and efficient surface meshing. It should be possible to modify the world without rebuilding the entire terrain.

### Support for conventional 3D scenes

The engine should also handle ordinary meshes, textures, materials, cameras, and lighting. Voxel-specific assumptions should remain inside the voxel subsystem.

### Reliable development workflows

Builds should be reproducible, errors should be diagnosable, and important systems should have automated tests where practical. Performance claims should come from measurements rather than assumptions.

### A useful open-source project

The goal is to build software that other developers can inspect, compile, learn from, and eventually contribute to. Documentation and code quality are part of the project, not tasks to postpone indefinitely.

## Planned Features

The features below describe the intended direction of development. Their availability will depend on completed milestones.

### Rendering

- Vulkan-based rendering foundation
- Camera and view/projection transforms
- Vertex and index buffers
- Mesh and texture resources
- Basic lighting
- Material and shader management
- GPU resource lifetime management
- Rendering diagnostics and profiling

### Voxel worlds

- Editable voxel chunk data
- Configurable chunk dimensions
- Procedural terrain generation
- Chunk surface meshing
- Mesh updates after terrain edits
- Efficient handling of neighboring chunk boundaries
- A path toward chunk streaming and level of detail (LOD)

### Engine systems

- Platform abstraction for supported desktop systems
- Core utilities and application lifecycle
- Scene and entity organization
- Asset loading and resource management
- Configuration and logging
- Automated tests for core algorithms
- Reproducible build configuration

### Future directions

Once the foundational systems are stable, potential areas of development include:

- Physically based rendering (PBR)
- Skeletal animation
- Physics integration
- Asynchronous chunk generation and streaming
- Terrain LOD and more advanced visibility techniques
- Editor and debugging tools
- More sophisticated materials and lighting
- Additional rendering techniques as real requirements emerge

These are longer-term possibilities, not commitments to implement every feature. Each addition should have a clear purpose and fit the existing architecture.

## Architecture

TerraCore is intended to use a modular architecture with dependencies flowing toward well-defined interfaces. The exact module boundaries will be refined as the implementation develops.

```text
TerraCore
├── Platform
│   ├── Window and input
│   └── OS-specific integration
├── Core
│   ├── Application lifecycle
│   ├── Logging and utilities
│   └── Configuration
├── Renderer
│   ├── Vulkan device and queues
│   ├── GPU resources
│   ├── Shaders and materials
│   └── Render submission
├── Assets
│   ├── Asset loading
│   ├── Textures and meshes
│   └── Resource management
├── Scene
│   ├── Cameras
│   ├── Scene organization
│   └── Renderable objects
└── Voxel
    ├── Chunk storage
    ├── Terrain generation
    └── Surface meshing
```

This is a logical view of the intended modules, not a claim that every directory or subsystem has already been implemented.

### Platform

Owns window creation, input, and operating-system integration. Platform-specific code should remain isolated so that supported operating systems can share the majority of the engine.

### Core

Contains foundational utilities and application-level infrastructure. It should not depend on the voxel system or on high-level rendering features.

### Renderer

Owns Vulkan initialization, GPU resources, shaders, and rendering operations. Vulkan-specific implementation details should remain within the renderer as much as practical.

### Assets

Handles loading and managing resources such as meshes and textures. Resource ownership and lifetime must be explicit, particularly when GPU resources are involved.

### Scene

Represents the objects and cameras that make up a scene. It provides the common layer through which conventional meshes and voxel-generated geometry can be presented to the renderer.

### Voxel

Owns voxel data, chunk operations, terrain generation, and surface extraction. Its responsibilities should remain separate from general mesh rendering.

### Architectural direction

The goal is not to introduce abstraction for its own sake. Interfaces should be added where they reduce coupling, clarify ownership, make testing easier, or provide a genuine path for future development.

## Technology Choices

| Technology | Intended role |
|---|---|
| C++ | Primary implementation language |
| Vulkan | Low-level graphics API |
| CMake | Cross-platform build configuration |
| Windows | Initial desktop platform |
| Linux | Initial desktop platform |
| Git | Version control and collaboration |

Additional libraries will be selected as implementation needs become clear. Windowing, mathematics, image loading, testing, and Vulkan development utilities are potential areas where established libraries can save time and reduce unnecessary maintenance.

Dependencies will be evaluated for their functionality, maintenance, platform support, integration cost, and licensing terms before being adopted.

## Development Roadmap

Development will proceed in stages. Later milestones depend on the stability of earlier ones.

### Phase 1 — Project foundation

- [ ] Establish the repository structure
- [ ] Configure CMake and the C++ standard
- [ ] Set up the development workflow for Windows and Linux
- [ ] Integrate the minimum required dependencies
- [ ] Establish basic logging and error handling
- [ ] Add an initial automated test target
- [ ] Document build prerequisites

**Completion criteria:** A clean checkout can be configured and built using documented instructions on the supported development platforms.

### Phase 2 — Vulkan renderer

- [ ] Create a window and Vulkan instance
- [ ] Select a physical device and create a logical device
- [ ] Configure queues and surface presentation
- [ ] Implement swapchain management
- [ ] Render a basic frame
- [ ] Add a camera
- [ ] Render a textured mesh
- [ ] Introduce basic lighting
- [ ] Add Vulkan validation and diagnostic workflows

**Completion criteria:** The engine can open a window and render a basic 3D scene reliably, with graphics validation enabled during development.

### Phase 3 — Scene and asset foundations

- [ ] Establish mesh and material representations
- [ ] Implement basic asset loading
- [ ] Define scene and renderable-object interfaces
- [ ] Clarify ownership of CPU and GPU resources
- [ ] Add examples for ordinary 3D geometry
- [ ] Test resource creation, replacement, and cleanup

**Completion criteria:** A scene can load and render conventional 3D assets through reusable engine systems.

### Phase 4 — Voxel terrain

- [ ] Define voxel and chunk data structures
- [ ] Implement chunk storage and indexing
- [ ] Generate initial terrain procedurally
- [ ] Implement surface meshing
- [ ] Handle chunk boundaries correctly
- [ ] Upload generated meshes to the renderer
- [ ] Support basic terrain edits
- [ ] Update affected geometry after edits

**Completion criteria:** A procedural voxel world can be rendered and edited without requiring a full-world rebuild for each local change.

### Phase 5 — Reliability and performance

- [ ] Add tests for voxel indexing and meshing
- [ ] Measure terrain generation and mesh-building costs
- [ ] Profile CPU and GPU rendering work
- [ ] Identify unnecessary allocations and copies
- [ ] Improve resource lifetime handling
- [ ] Add reproducible benchmark scenarios
- [ ] Document known limitations and measured results

**Completion criteria:** Core algorithms have meaningful tests, and performance decisions can be supported by repeatable measurements.

### Phase 6 — Integrated demonstration

- [ ] Build a voxel-world demonstration
- [ ] Build a conventional 3D scene demonstration
- [ ] Exercise shared renderer and asset systems
- [ ] Document build and run instructions
- [ ] Publish screenshots or a recorded demonstration
- [ ] Record the tested platforms and known issues

**Completion criteria:** Both types of scene run through the same engine foundation, demonstrating that the architecture serves more than one rendering use case.

Streaming, LOD, PBR, animation, physics, and editor tools will be considered after the core systems have demonstrated that they are worth extending.

## Build and Development

TerraCore is planned to use CMake, with Vulkan SDK requirements documented as the renderer becomes available.

Because the project is in its early stages, build commands and dependency requirements will be documented alongside the corresponding implementation. Instructions should always match the actual state of the repository.

The intended workflow is:

1. Install the documented compiler, CMake version, and platform dependencies.
2. Install the required Vulkan SDK when building renderer components.
3. Configure the project using CMake.
4. Build the selected targets.
5. Run the available tests.
6. Launch the relevant example or demonstration.

Example commands for a configured CMake project will follow this general pattern:

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

These commands describe the intended workflow. They are not a guarantee that the current repository already contains the corresponding targets.

Exact compiler versions, SDK versions, generator choices, and platform-specific setup instructions will be recorded as they are established.

## Design Principles

### Correctness before optimization

Get the behavior right, test it, and measure it before adding complexity for performance.

### Explicit resource ownership

GPU resources, asset data, and asynchronous work need clear ownership and lifetime rules. Undefined ownership is a source of difficult bugs in long-lived engine projects.

### Keep subsystems independent

Voxel terrain should use the engine's common rendering facilities rather than dictate how every scene works. Likewise, the renderer should not need to understand how terrain was generated.

### Prefer measured complexity

Advanced systems should be introduced when there is a concrete need. Prematurely implementing an elaborate job system, editor framework, or rendering abstraction creates maintenance costs before their value is known.

### Make the project reproducible

Build instructions, dependency versions, tests, and benchmark procedures should make it possible for another developer to reproduce important results.

### Document decisions

Significant architectural choices should include the problem being solved, alternatives considered, and the trade-offs involved. This makes future changes easier to reason about.

## Testing and Performance

Testing will be introduced alongside the systems it is intended to protect.

Examples of useful test areas include:

- Voxel coordinate conversion and chunk indexing
- Chunk-boundary and neighbor handling
- Mesh generation for empty, solid, and mixed voxel data
- Terrain generation with repeatable inputs
- Resource-management and cleanup paths
- Scene and asset-loading error handling
- Build and integration checks

Performance work will focus on measurable costs, including CPU time spent generating terrain and meshes, memory use, GPU frame time, draw submission, and the cost of updating edited chunks.

Benchmarks should document their hardware, software configuration, workload, and measurement method. Results should not be compared without accounting for differences in conditions.

## Contributing

TerraCore is intended to be developed in the open, with room for contributors as the project matures.

Before contributing, check the existing implementation, open issues, and roadmap to understand the current direction. For substantial changes, discussing the proposed design before writing a large implementation can help avoid incompatible approaches.

Useful contributions include:

- Bug reports with reproducible steps
- Focused fixes with tests
- Build and platform improvements
- Documentation corrections
- Small, well-scoped architectural improvements
- Profiling results and reproducible performance investigations

Contribution guidelines, issue templates, and pull-request expectations will be added as the repository and development workflow take shape.

## Licensing

The intended licensing direction is an open-source engine core, with Apache License 2.0 under consideration.

The final license will be established after reviewing dependencies and their compatibility. Third-party libraries, sample assets, fonts, datasets, and other bundled materials may carry separate license terms.

Until the repository contains the finalized license and applicable notices, do not assume that the intended license has already been formally applied.

The long-term aim is to keep the engine core useful on its own. Optional professional tools or support may be considered if there is demonstrated demand, but commercial plans should not compromise the usefulness of the open-source foundation.

## Project Status

TerraCore is in its early development and planning stage.

The initial focus is on establishing a reliable build system, bringing up Vulkan, and creating a small renderer that can serve as the foundation for the rest of the engine.

The roadmap is a direction for development, not a list of completed features. Implemented functionality, tested platforms, known limitations, and release notes should be updated as the project progresses.

There is no promise of production readiness or a fixed release date.

## Long-Term Direction

The long-term ambition is to develop TerraCore into a capable engine for both voxel-based and conventional 3D games, with room to explore more advanced graphics, larger worlds, and the tools needed to build complete projects.

That ambition will be pursued one working subsystem at a time.

A small renderer that builds reliably, produces correct results, and can be understood by its maintainers is a better starting point than a large collection of unfinished systems.

---

**TerraCore is built incrementally, with an emphasis on sound engineering, useful documentation, and results that can be reproduced.**
