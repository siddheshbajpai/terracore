# Project: TerraCore

A modular, open-source C++ 3D engine using Vulkan, initially targeting Windows
and Linux. The long-term vision is to support both Minecraft-like voxel worlds
and conventional 3D games, eventually progressing toward advanced story-driven
games.

## MVP (this release)

- CMake project with reproducible builds
- Vulkan renderer (context, swapchain, pipelines, command buffers)
- Camera, meshes, textures, basic lighting
- Editable voxel chunks
- Procedural terrain + chunk meshing
- Scene management (entities, components, scene manager)
- Profiling
- Test suite
- Reproducible builds

## Architecture

```
platform/   - OS/window abstraction (GLFW backend)
core/       - logging, timing, math
renderer/   - Vulkan context, swapchain, pipelines, meshes, textures, materials, profiler
assets/     - asset manager, image loading (stb)
scene/      - ECS-style entities/components, camera, light, scene manager
voxel/      - voxel types, chunks, greedy chunk meshing, voxel world, terrain
app/        - demo application (voxel world + conventional scene)
tests/      - unit tests
```

## Dependencies

| Dependency | Role | Required |
|---|---|---|
| Vulkan SDK | Graphics API | Yes (renderer) |
| CMake 3.15+ | Build system | Yes |
| C++14 compiler | GCC/Clang/MSVC | Yes |
| GLFW 3.3+ | Windowing/surface | Optional (recommended) |
| stb_image | Image decoding | Vendored (header-only) |

## Build

### Prerequisites

Install the Vulkan SDK for your platform:
- Windows: https://vulkan.lunarg.com/sdk/home
- Linux: your distro's `vulkan-sdk` / `libvulkan-dev` package

Install GLFW (optional):
- Windows: `vcpkg install glfw3` or build from source
- Linux: `sudo apt install libglfw3-dev`

### Configure & build (single-config, e.g. Linux/MinGW)

```bash
cd TerraCore
mkdir build && cd build
cmake .. -DTERRA_CORE_BUILD_TESTS=ON -DTERRA_CORE_BUILD_DEMO=ON
cmake --build . -j$(nproc)
ctest --output-on-failure
```

### Configure & build (multi-config, e.g. Visual Studio)

```bash
cd TerraCore
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Reproducible builds

Pin dependency versions (GLFW tag is locked in `CMakeLists.txt`) and use a
fixed toolchain. Vendored headers under `third_party/` avoid network fetches.
Set `TERRA_CORE_VENDOR_DEPS=ON` and `TERRA_CORE_USE_GLFW=OFF` with a local
GLFW checkout for fully offline, reproducible builds.

## Demo

Run `terra_demo` to see a reproducible scene containing both a voxel world and
a conventional 3D scene.

## License

Apache-2.0 (see `LICENSE`). Dependency review pending for downstream use.
