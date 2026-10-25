# ---------------------------------------------------------------------------
# TerraCore tests
#
# Built without Vulkan: only pure-C++ logic (core, scene, voxel) is exercised,
# so the suite runs on any C++14 toolchain with no graphics SDK required.
# ---------------------------------------------------------------------------

set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_math.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_entity.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_voxel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_chunk.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_terrain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_voxel_world.cpp"
)

# Source files pulled straight into the test binary (no Vulkan dependency).
set(LOGIC_SOURCES
    "${TERRA_CORE_SOURCE_DIR}/core/log.cpp"
    "${TERRA_CORE_SOURCE_DIR}/core/timer.cpp"
    "${TERRA_CORE_SOURCE_DIR}/core/math.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/transform.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/entity.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/camera.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/light.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/scene_manager.cpp"
    "${TERRA_CORE_SOURCE_DIR}/scene/scene.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/voxel.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/chunk.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/chunk_mesh.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/voxel_world.cpp"
    "${TERRA_CORE_SOURCE_DIR}/voxel/terrain.cpp"
)

add_executable(terra_tests ${TEST_SOURCES} ${LOGIC_SOURCES})
target_include_directories(terra_tests PRIVATE
    "${TERRA_CORE_INCLUDE_DIR}"
    "${TERRA_CORE_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(terra_tests PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(terra_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_test(NAME terra_tests COMMAND terra_tests)
