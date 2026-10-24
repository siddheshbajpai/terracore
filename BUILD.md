# TerraCore Build Guide

## 1. Install the Vulkan SDK

The renderer links against `Vulkan::Vulkan` (CMake's `FindVulkan`). You must
have the Vulkan SDK installed with `VULKAN_SDK` pointing at it, or a system
Vulkan installation.

- Windows: https://vulkan.lunarg.com/sdk/home
- Linux (Debian/Ubuntu): `sudo apt install libvulkan-dev vulkan-validationlayers`

## 2. Configure

From the `TerraCore` directory:

```bash
mkdir build && cd build
cmake .. \
  -DTERRA_CORE_BUILD_TESTS=ON \
  -DTERRA_CORE_BUILD_DEMO=ON \
  -DTERRA_CORE_USE_GLFW=ON \
  -DCMAKE_BUILD_TYPE=Release
```

### Offline / reproducible

If you cannot fetch GLFW from the network, checkout GLFW locally and point CMake
at it:

```bash
cmake .. \
  -DTERRA_CORE_USE_GLFW=ON \
  -DTERRA_CORE_VENDOR_DEPS=ON \
  -DTERRA_CORE_GLFW_DIR=/path/to/local/glfw
```

To disable GLFW entirely (headless / no windowing):

```bash
cmake .. -DTERRA_CORE_USE_GLFW=OFF
```

## 3. Build

Single-config (Unix Makefiles, MinGW):

```bash
cmake --build . -j
```

Multi-config (Visual Studio):

```bash
cmake --build . --config Release
```

## 4. Test

```bash
ctest --output-on-failure
```

## 5. Run the demo

```bash
./terra_demo          # Linux/MinGW
# or
.\Debug\terra_demo.exe   # Visual Studio
```

## Compiler notes

- g++ 6.x (MinGW): C++14 is used deliberately for broad compatibility.
- MSVC: `/W4 /WX /permissive-` enforced.
- GCC/Clang: `-Wall -Wextra -Wpedantic -Werror` enforced.
