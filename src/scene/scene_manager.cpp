#include "terra/scene/scene_manager.h"

#include <algorithm>

#include "terra/core/log.h"

namespace terra {

SceneManager::SceneManager()
    : m_nextEntityId(1)
    , m_mainCamera(nullptr) {}

SceneManager::~SceneManager() {
    for (auto* e : m_entities) delete e;
    for (auto* c : m_cameras) delete c;
    for (auto* l : m_lights) delete l;
}

Entity* SceneManager::createEntity(std::string name) {
    Entity* e = new Entity(m_nextEntityId++, std::move(name));
    m_entities.push_back(e);
    m_entityMap[e->id()] = e;
    return e;
}

Entity* SceneManager::getEntity(Entity::Id id) {
    auto it = m_entityMap.find(id);
    return it != m_entityMap.end() ? it->second : nullptr;
}

const Entity* SceneManager::getEntity(Entity::Id id) const {
    auto it = m_entityMap.find(id);
    return it != m_entityMap.end() ? it->second : nullptr;
}

void SceneManager::destroyEntity(Entity::Id id) {
    auto it = m_entityMap.find(id);
    if (it == m_entityMap.end()) return;
    Entity* e = it->second;

    // Detach from parent via the public API (setParent detaches first).
    if (e->parent()) {
        e->setParent(nullptr);
    }
    // Reparent children to nullptr (they stay alive, detached).
    // Copy first because setParent(nullptr) mutates e->m_children.
    std::vector<Entity*> kids = e->children();
    for (auto* child : kids) {
        child->setParent(nullptr);
    }

    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), e),
                     m_entities.end());
    m_entityMap.erase(it);
    delete e;
}

Camera* SceneManager::mainCamera() { return m_mainCamera; }

const Camera* SceneManager::mainCamera() const { return m_mainCamera; }

void SceneManager::setMainCamera(Camera* cam) { m_mainCamera = cam; }

Camera* SceneManager::createCamera(std::string name) {
    (void)name;
    Camera* cam = new Camera();
    m_cameras.push_back(cam);
    if (!m_mainCamera) m_mainCamera = cam;
    return cam;
}

Light* SceneManager::createLight(Light::Type type, std::string name) {
    (void)name;
    Light* light = new Light();
    light->type = type;
    m_lights.push_back(light);
    return light;
}

void SceneManager::update(float dt) {
    (void)dt;
    // Per-frame scene updates (animation, scripts) go here in later
    // milestones.
}

} // namespace terra
