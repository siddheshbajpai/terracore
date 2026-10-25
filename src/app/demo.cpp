#include "demo.h"

#include <iostream>

#include "terra/core/log.h"
#include "terra/core/math.h"
#include "terra/assets/asset_manager.h"

namespace {

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

} // namespace

Demo::Demo()
    : m_window()
    , m_running(false) {}

Demo::~Demo() {}

bool Demo::init() {
    TERRA_LOG_INFO("TerraCore demo starting");

    if (!m_window.create("TerraCore Demo", kWindowWidth, kWindowHeight)) {
        TERRA_LOG_ERROR("failed to create window");
        return false;
    }

    if (!m_renderer.init(m_window)) {
        TERRA_LOG_ERROR("failed to initialize renderer");
        return false;
    }

    // Scene: main camera + a light.
    terra::Camera* cam = m_scene.createCamera("MainCamera");
    cam->setPerspective(60.0f,
                        (float)kWindowWidth / (float)kWindowHeight, 0.1f,
                        1000.0f);
    cam->setLookAt(terra::vec3(0, 5, 12), terra::vec3(0, 0, 0),
                terra::vec3(0, 1, 0));
    m_scene.setMainCamera(cam);

    terra::Light* light = m_scene.createLight(terra::Light::Directional,
                                              "Sun");
    light->direction = terra::vec3(-0.5f, -1.0f, -0.3f);
    light->color = terra::vec3(1, 1, 1);
    light->intensity = 1.0f;

    // Conventional scene entity (a placeholder mesh would go here).
    terra::Entity* worldEnt = m_scene.createEntity("ConventionalScene");
    worldEnt->transform().position = terra::vec3(0, 0, 0);

    // Voxel world: generate terrain around the origin.
    m_terrain.setSeed(20261025u);
    for (int z = -1; z <= 1; ++z) {
        for (int x = -1; x <= 1; ++x) {
            terra::Chunk* chunk = m_voxelWorld.ensureChunk(x, z);
            m_terrain.generate(*chunk);
        }
    }

    TERRA_LOG_INFO("demo initialized");
    m_running = true;
    return true;
}

int Demo::run() {
    while (!m_window.shouldClose() && m_running) {
        m_window.pollEvents();
        float dt = 0.016f; // fixed step for MVP; real delta from timer later
        update(dt);
        render();
    }
    return 0;
}

void Demo::update(float dt) {
    (void)dt;
    m_scene.update(dt);
}

void Demo::render() {
    if (!m_renderer.beginFrame()) return;
    m_renderer.drawFrame();
    m_renderer.endFrame();
}
