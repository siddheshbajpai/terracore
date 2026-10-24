cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(TerraCore
    VERSION 0.1.0
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Project: TerraCore
# Modular, open-source C++ 3D engine using Vulkan.
#
# Modules: platform, core, renderer, assets, scene, voxel, app, tests
# ---------------------------------------------------------------------------

set(TERRA_CORE_VERSION_MAJOR 0)
set(TERRA_CORE_VERSION_MINOR 1)
set(TERRA_CORE_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /permissive-")
    add_compile_options($<$<CONFIG:Release>:/O2>)
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
    add_compile_options($<$<CONFIG:Release>:-O3>)
endif()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(TERRA_CORE_BUILD_TESTS "Build the test suite" ON)
option(TERRA_CORE_BUILD_DEMO  "Build the demo application" ON)
option(TERRA_CORE_USE_GLFW   "Use GLFW for windowing/surface creation" ON)
option(TERRA_CORE_VENDOR_DEPS "Vendor header-only dependencies (stb)" ON)

set(TERRA_CORE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TERRA_CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TERRA_CORE_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# ---------------------------------------------------------------------------
# GNUInstallDirs
# ---------------------------------------------------------------------------
include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
# Vulkan is required by the renderer. On Windows/Linux the user must have the
# Vulkan SDK installed (VULKAN_SDK set) or a system Vulkan available.
find_package(Vulkan REQUIRED)

# GLFW for windowing + surface creation (optional but recommended).
if(TERRA_CORE_USE_GLFW)
    find_package(glfw3 3.3 QUIET)
    if(glfw3_FOUND)
        set(TERRA_CORE_HAS_GLFW ON)
        message(STATUS "TerraCore: GLFW found (system)")
    else()
        message(STATUS "TerraCore: GLFW not found system-side; attempting FetchContent")
        include(FetchContent)
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG        3.4
        )
        # For reproducible / offline builds, disable FetchContent and point
        # TERRA_CORE_GLFW_DIR at a local checkout instead.
        set(TERRA_CORE_GLFW_DIR "" CACHE PATH "Path to a local GLFW checkout")
        if(TERRA_CORE_GLFW_DIR)
            add_subdirectory("${TERRA_CORE_GLFW_DIR}" EXCLUDE_FROM_ALL)
            set(TERRA_CORE_HAS_GLFW ON)
            set(GLFW_LIBRARY glfw)
        else()
            FetchContent_Populate(glfw)
            add_subdirectory("${glfw_SOURCE_DIR}" "${glfw_BINARY_DIR}" EXCLUDE_FROM_ALL)
            set(TERRA_CORE_HAS_GLFW ON)
            set(GLFW_LIBRARY glfw)
        endif()
    endif()
    if(TERRA_CORE_HAS_GLFW)
        find_package(Threads REQUIRED)
    endif()
endif()

# Header-only dependencies (vendored when network is unavailable).
if(TERRA_CORE_VENDOR_DEPS)
    set(TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS "${TERRA_CORE_THIRD_PARTY_DIR}")
endif()

# ---------------------------------------------------------------------------
# Public include directories
# ---------------------------------------------------------------------------
set(TERRA_CORE_PUBLIC_INCLUDES
    "${TERRA_CORE_INCLUDE_DIR}"
    "${TERRA_CORE_SOURCE_DIR}"
)

# ---------------------------------------------------------------------------
# Core library (no Vulkan dependency)
# ---------------------------------------------------------------------------
add_library(terra_core STATIC
    "${TERRA_CORE_SOURCE_DIR}/core/log.cpp"
    "${TERRA_CORE_SOURCE_DIR}/core/timer.cpp"
    "${TERRA_CORE_SOURCE_DIR}/core/math.cpp"
    "${TERRA_CORE_SOURCE_DIR}/platform/platform.cpp"
)
target_include_directories(terra_core PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_compile_definitions(terra_core PUBLIC
    TERRA_CORE_VERSION_MAJOR=${TERRA_CORE_VERSION_MAJOR}
    TERRA_CORE_VERSION_MINOR=${TERRA_CORE_VERSION_MINOR}
    TERRA_CORE_VERSION_PATCH=${TERRA_CORE_VERSION_PATCH}
)

# glfw_window.cpp (Vulkan + GLFW surface helpers) belongs to the renderer so
# that terra_core stays Vulkan-free.
add_library(terra_renderer STATIC
    "${TERRA_CORE_SOURCE_DIR}/platform/glfw_window.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/vulkan_context.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/swapchain.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/pipeline.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/mesh.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/texture.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/material.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/command_buffer.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/profiler.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/renderer.cpp"
)
target_include_directories(terra_renderer PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_link_libraries(terra_renderer PUBLIC
    terra_core
    Vulkan::Vulkan
)
if(TERRA_CORE_USE_GLFW AND TERRA_CORE_HAS_GLFW)
    target_compile_definitions(terra_renderer PUBLIC TERRA_CORE_USE_GLFW=1)
    target_link_libraries(terra_renderer PUBLIC ${GLFW_LIBRARY} ${GLFW_LIBRARIES} Threads::Threads)
endif()

# ---------------------------------------------------------------------------
# Renderer library (Vulkan)
# ---------------------------------------------------------------------------
add_library(terra_renderer STATIC
    "${TERRA_CORE_SOURCE_DIR}/renderer/vulkan_context.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/swapchain.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/pipeline.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/mesh.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/texture.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/material.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/command_buffer.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/profiler.cpp"
    "${TERRA_CORE_SOURCE_DIR}/renderer/renderer.cpp"
)
target_include_directories(terra_renderer PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_link_libraries(terra_renderer PUBLIC
    terra_core
    Vulkan::Vulkan
)
if(TERRA_CORE_USE_GLFW AND TERRA_CORE_HAS_GLFW)
    target_link_libraries(terra_renderer PUBLIC ${GLFW_LIBRARY})
endif()

# ---------------------------------------------------------------------------
# Assets library
# ---------------------------------------------------------------------------
add_library(terra_assets STATIC
    "${TERRA_CORE_SOURCE_DIR}/assets/asset_manager.cpp"
    "${TERRA_CORE_SOURCE_DIR}/assets/image.cpp"
)
target_include_directories(terra_assets PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_link_libraries(terra_assets PUBLIC terra_core)

# ---------------------------------------------------------------------------
# Scene library
# ---------------------------------------------------------------------------
add_library(terra_scene STATIC
    "${TERRA_CORE_SOURCE_DIR}/scene/scene.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/entity.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/transform.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/camera.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/light.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/scene_manager.cpp"
)
target_include_directories(terra_scene PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_link_libraries(terra_scene PUBLIC terra_core terra_assets)

# ---------------------------------------------------------------------------
# Voxel library
# ---------------------------------------------------------------------------
add_library(terra_voxel STATIC
    "${TERRA_CORE_SOURCE_DIR}/voxel/voxel.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/chunk.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/chunk_mesh.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/voxel_world.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/terrain.cpp"
)
target_include_directories(terra_voxel PUBLIC
    "${TERRA_CORE_PUBLIC_INCLUDES}"
    "${TERRA_CORE_THIRD_PARTY_INCLUDE_DIRS}"
)
target_link_libraries(terra_voxel PUBLIC terra_core terra_scene)

# ---------------------------------------------------------------------------
# Demo application
# ---------------------------------------------------------------------------
if(TERRA_CORE_BUILD_DEMO)
    add_executable(terra_demo
        "${TERRA_CORE_SOURCE_DIR}/app/main.cpp"
        "${TERRA_CORE_SOURCE_DIR}/app/demo.cpp"
    )
    target_link_libraries(terra_demo PRIVATE
        terra_renderer
        terra_assets
        terra_scene
        terra_voxel
        terra_core
    )
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(TERRA_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS terra_core terra_renderer terra_assets terra_scene terra_voxel
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h")
