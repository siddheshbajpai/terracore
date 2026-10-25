#pragma once

#include <string>

#include "terra/scene/camera.h"
#include "terra/scene/entity.h"
#include "terra/scene/light.h"
#include "terra/scene/scene_manager.h"

namespace terra {

// Abstract scene interface. Concrete scenes are built on top of SceneManager.
// Kept minimal for the MVP; extended as the engine grows.
class Scene {
public:
    virtual ~Scene() {}

    virtual const std::string& name() const = 0;

    virtual SceneManager& manager() = 0;
    virtual const SceneManager& manager() const = 0;

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float dt) { (void)dt; }
};

} // namespace terra
