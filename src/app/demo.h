#pragma once

#include <string>

#include "terra/renderer/renderer.h"
#include "terra/scene/scene_manager.h"
#include "terra/scene/camera.h"
#include "terra/voxel/voxel_world.h"
#include "terra/voxel/terrain.h"
#include "terra/platform/glfw_window.h"

// The demo application: renders both a conventional 3D scene and a voxel
// world side by side through the shared TerraCore renderer.
class Demo {
public:
    Demo();
    ~Demo();

    // Initialize window, Vulkan renderer, scene, assets and voxel world.
    bool init();

    // Run the main loop until the window closes.
    int run();

private:
    void update(float dt);
    void render();

    terra::Renderer m_renderer;
    terra::GlfwWindow m_window;
    terra::SceneManager m_scene;
    terra::VoxelWorld m_voxelWorld;
    terra::Terrain m_terrain;
    bool m_running;
};
