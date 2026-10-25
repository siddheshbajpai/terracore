#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "terra/scene/camera.h"
#include "terra/scene/entity.h"
#include "terra/scene/light.h"

namespace terra {

// Owning scene manager: entities, cameras and lights with a single main
// camera. Simple container-style scene graph for the MVP.
class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Create and own a new entity. Returns non-owning pointer.
    Entity* createEntity(std::string name = "Entity");

    Entity* getEntity(Entity::Id id);
    const Entity* getEntity(Entity::Id id) const;

    // Remove and destroy an entity (and detach it from its parent).
    void destroyEntity(Entity::Id id);

    const std::vector<Entity*>& entities() const { return m_entities; }

    Camera* mainCamera();
    const Camera* mainCamera() const;
    void setMainCamera(Camera* cam);

    // Create and own a camera.
    Camera* createCamera(std::string name = "Camera");
    const std::vector<Camera*>& cameras() const { return m_cameras; }

    // Create and own a light.
    Light* createLight(Light::Type type = Light::Directional,
                       std::string name = "Light");
    const std::vector<Light*>& lights() const { return m_lights; }

    // Advance the scene (entity transforms, animations, etc.).
    void update(float dt);

private:
    std::vector<Entity*> m_entities;
    std::unordered_map<Entity::Id, Entity*> m_entityMap;
    Entity::Id m_nextEntityId;

    std::vector<Camera*> m_cameras;
    std::vector<Light*> m_lights;
    Camera* m_mainCamera;
};

} // namespace terra
