# TerraCore

**A modular C++ 3D engine built with Vulkan, with a long-term focus on voxel worlds and conventional 3D games.**

TerraCore is an early-stage open-source engine project exploring the foundations of real-time 3D rendering, reusable engine systems, and procedural environments. The goal is to support editable voxel terrain and conventional 3D scenes through a shared rendering foundation.

Development is intentionally incremental: establish a reliable build, bring up Vulkan, and build a small working renderer before expanding into terrain, scene management, and higher-level tools.

> **Project status:** Early development. Features in this README describe goals and planned work; they should not be interpreted as implemented or production-ready functionality.

---

## Contents

- [Goals](#goals)
- [Planned capabilities](#planned-capabilities)
- [Architecture](#architecture)
- [Technology](#technology)
- [Roadmap](#roadmap)
- [Build and development](#build-and-development)
- [Engineering principles](#engineering-principles)
- [Testing and performance](#testing-and-performance)
- [Contributing](#contributing)
- [Licensing](#licensing)
- [Current status](#current-status)
- [Long-term direction](#long-term-direction)

## Goals

TerraCore aims to provide a reusable foundation for experimenting with and eventually building 3D games.

- **A maintainable architecture.** Keep platform integration, rendering, asset handling, scene management, and voxel terrain in clearly defined subsystems.
- **A renderer that can grow.** Start with a minimal Vulkan renderer and expand based on demonstrated needs rather than speculative feature lists.
- **Voxel terrain as a first-class use case.** Work toward editable chunk data, procedural generation, and efficient surface meshing.
- **Conventional 3D scenes.** Support ordinary meshes, textures, materials, cameras, and lighting without making the entire engine depend on voxel-specific concepts.
- **Reliable development.** Favor reproducible builds, useful diagnostics, automated tests, and performance measurements.
- **An approachable open-source codebase.** Make the implementation understandable, documented, and practical for other developers to build and contribute to.

## Planned capabilities

These are development goals, not a claim that the features are currently available.

### Rendering

- Vulkan initialization, device selection, and presentation
- Camera and view/projection transforms
- Vertex and index buffers
- Mesh and texture resources
- Basic lighting, materials, and shader management
- Explicit GPU resource lifetime management
- Rendering diagnostics and profiling

### Voxel worlds

- Voxel data organized into chunks
- Configurable chunk dimensions
- Procedural terrain generation
- Surface meshing and mesh updates after edits
- Correct handling of neighboring chunk boundaries
- A future path toward chunk streaming and level of detail (LOD)

### General engine systems

- Desktop platform and application lifecycle
- Scene and renderable-object organization
- Asset loading and resource management
- Configuration, logging, and error handling
- Automated tests for core algorithms
- Documented, reproducible build workflows

### Possible later work

Once the core is stable, areas worth evaluating include physically based rendering (PBR), animation, physics integration, asynchronous terrain generation, LOD, editor tools, and more advanced rendering techniques. These are possibilities rather than commitments; each should solve a real problem and justify its maintenance cost.

## Architecture

The intended design separates general engine infrastructure from voxel-specific behavior. Exact module boundaries will be refined as the implementation develops.

```text
TerraCore
├── Platform   Windowing, input, operating-system integration
├── Core       Application lifecycle, logging, configuration, utilities
├── Renderer   Vulkan device, GPU resources, shaders, render submission
├── Assets     Mesh and texture loading, resource management
├── Scene      Cameras, scene organization, renderable objects
└── Voxel      Chunk storage, terrain generation, surface meshing
```

This is a logical architecture sketch, not a description of existing directories or completed systems.

| Subsystem | Responsibility |
|---|---|
| **Platform** | Own window creation, input, and OS-specific integration while keeping platform-dependent code isolated. |
| **Core** | Provide foundational utilities and application infrastructure without depending on high-level engine features. |
| **Renderer** | Own Vulkan initialization, GPU resources, shaders, and rendering operations. |
| **Assets** | Load and manage resources such as meshes and textures, with clear ownership and lifetime rules. |
| **Scene** | Represent cameras and scene objects and provide a common route for geometry to reach the renderer. |
| **Voxel** | Own voxel data, chunk operations, terrain generation, and surface extraction without dictating how ordinary scenes work. |

Abstraction should be introduced when it reduces coupling, clarifies ownership, improves testability, or supports a concrete future requirement—not for its own sake.

## Technology

| Technology | Intended role |
|---|---|
| C++ | Primary implementation language |
| Vulkan | Low-level graphics API |
| CMake | Cross-platform build configuration |
| Windows and Linux | Initial desktop platforms |
| Git | Version control and collaboration |

Additional dependencies will be selected as implementation needs become clear. Windowing, mathematics, image loading, testing, and Vulkan development tools are areas where established libraries may be preferable to custom implementations.

Dependencies will be assessed for functionality, maintenance, platform support, integration cost, and license compatibility before adoption. The licensing of third-party code and bundled assets will be considered separately from TerraCore's own license.

## Roadmap

The roadmap is ordered by dependency: later stages should build on working, tested foundations. Progress should be reflected in the repository as implementation becomes available.

### Phase 1 — Project foundation

- [ ] Establish the repository and source layout
- [ ] Configure CMake and select the C++ standard
- [ ] Document the Windows and Linux development setup
- [ ] Integrate the minimum required dependencies
- [ ] Add basic logging and error handling
- [ ] Add an initial automated test target

**Exit criteria:** A clean checkout can be configured and built using documented instructions on the supported development platforms.

### Phase 2 — Vulkan renderer

- [ ] Create a window and Vulkan instance
- [ ] Select a physical device and create a logical device
- [ ] Configure queues, surface presentation, and swapchain management
- [ ] Render a basic frame
- [ ] Add a camera and render a textured mesh
- [ ] Introduce basic lighting
- [ ] Document Vulkan validation and diagnostic workflows

**Exit criteria:** The application reliably opens a window and renders a basic 3D scene, with validation enabled during development.

### Phase 3 — Scene and asset foundations

- [ ] Define mesh and material representations
- [ ] Implement basic asset loading
- [ ] Establish scene and renderable-object interfaces
- [ ] Clarify CPU and GPU resource ownership
- [ ] Add examples using ordinary 3D geometry
- [ ] Test resource creation, replacement, and cleanup

**Exit criteria:** A scene can load and render conventional 3D assets through reusable engine systems.

### Phase 4 — Voxel terrain

- [ ] Define voxel and chunk data structures
- [ ] Implement chunk storage and indexing
- [ ] Generate initial terrain procedurally
- [ ] Implement surface meshing
- [ ] Handle chunk boundaries and neighboring data
- [ ] Upload generated meshes to the renderer
- [ ] Support basic terrain edits and update affected geometry

**Exit criteria:** A procedural voxel world can be rendered and edited without rebuilding the entire world for every local change.

### Phase 5 — Reliability and performance

- [ ] Test voxel indexing, chunk boundaries, and meshing
- [ ] Measure terrain generation and mesh-building costs
- [ ] Profile CPU and GPU rendering work
- [ ] Investigate unnecessary allocations and copies
- [ ] Verify resource lifetime and cleanup paths
- [ ] Add repeatable benchmark scenarios
- [ ] Document methodology, results, and known limitations

**Exit criteria:** Important algorithms have meaningful tests, and performance decisions are backed by reproducible measurements.

### Phase 6 — Demonstrations

- [ ] Build a voxel-world demonstration
- [ ] Build a conventional 3D scene demonstration
- [ ] Exercise shared rendering and asset systems
- [ ] Document build and run instructions
- [ ] Publish screenshots or a recorded demo
- [ ] Record tested platforms and known issues

**Exit criteria:** Both scene types run on the same engine foundation, demonstrating that the architecture supports more than one use case.

Streaming, LOD, PBR, animation, physics, and editor tools should be considered after the foundational systems are working and their limitations are understood.

## Build and development

The project is intended to use CMake. Vulkan SDK requirements and other dependencies will be documented alongside the implementation that needs them.

The general development workflow will be:

1. Install the documented compiler, CMake version, and platform dependencies.
2. Install the Vulkan SDK when building renderer components.
3. Configure the project with CMake.
4. Build the required targets.
5. Run the available tests.
6. Launch the relevant example or demonstration.

A typical CMake workflow looks like this:

```bash
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

**These commands illustrate the intended workflow; they are not a guarantee that the current repository already contains the corresponding build targets.** Follow the actual build files and platform-specific instructions as they are added.

## Engineering principles

- **Correctness before optimization.** Establish expected behavior, test it, and measure it before adding performance-driven complexity.
- **Explicit ownership.** GPU resources, asset data, and asynchronous work need clear lifetime rules.
- **Separation of concerns.** The renderer should not need to know how terrain was generated, and ordinary scenes should not depend on voxel-specific logic.
- **Measured complexity.** Avoid building elaborate job systems, editor frameworks, or abstractions before there is a concrete reason.
- **Reproducibility.** Document build steps, dependency choices, test procedures, and benchmark conditions.
- **Recorded decisions.** For significant architectural choices, explain the problem, alternatives, and trade-offs so future changes remain understandable.

## Testing and performance

Testing should be introduced alongside the systems it protects. Useful areas include:

- Voxel coordinate conversion and chunk indexing
- Chunk boundaries and neighbor handling
- Mesh generation for empty, solid, and mixed voxel data
- Terrain generation with repeatable inputs
- Resource-management and cleanup paths
- Scene and asset-loading error handling
- Build and integration checks

Performance work should examine measurable costs such as CPU time spent generating terrain and meshes, memory use, GPU frame time, draw submission, and the cost of updating edited chunks.

Benchmarks should record the hardware, software configuration, workload, and measurement method. Results should not be compared without considering differences in test conditions. No performance claims should be made until they are supported by actual measurements.

## Contributing

TerraCore is intended to be developed in the open, with room for contributions as the project matures.

Before starting a substantial change, review the existing implementation and roadmap. For larger architectural work, opening an issue to discuss the design first can help avoid incompatible approaches.

Useful early contributions include:

- Reproducible bug reports
- Focused fixes accompanied by tests
- Build and platform improvements
- Documentation corrections
- Small, well-scoped architectural improvements
- Profiling results and reproducible performance investigations

Formal contribution guidelines, issue templates, and pull-request expectations can be added as the codebase and workflow take shape. Please avoid presenting planned features as completed functionality in issues, documentation, or demonstrations.

## Licensing

Apache License 2.0 is under consideration for the engine, but the final licensing decision has not been made in this README. The repository should include the finalized license and applicable notices before a license is represented as adopted.

Third-party libraries, sample assets, fonts, datasets, and other bundled materials may have separate terms. Their licenses and compatibility should be reviewed individually.

The long-term aim is to keep the engine core genuinely useful as open-source software. Optional professional tools or support may be explored if there is demonstrated demand, without making the core artificially limited.

## Current status

TerraCore is in its early development and planning stage. The immediate priority is a reliable project build, followed by a minimal Vulkan application and renderer.

At this stage, the roadmap communicates intended direction rather than completed functionality. As development progresses, this section should be updated with concrete information: what builds, what runs, which platforms have been tested, and what limitations remain.

There is no claim of production readiness or a fixed release date.

## Long-term direction

The ambition is to grow TerraCore into a capable engine for both voxel-based and conventional 3D games, with room to explore larger worlds, more advanced graphics, and the tools needed to build complete projects.

The way there is through working subsystems, clear boundaries, tests, and evidence—not through a large list of promises. A small renderer that builds reliably and can be understood by its maintainers is the right foundation for a much more ambitious engine.

---

*Built incrementally, with an emphasis on sound engineering, clear documentation, and reproducible results.*
