#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "terra/core/math.h"
#include "terra/scene/transform.h"

namespace terra {

class Entity {
public:
    using Id = uint32_t;

    Entity(Id id, std::string name);

    Id id() const { return m_id; }
    const std::string& name() const { return m_name; }
    void setName(std::string name) { m_name = std::move(name); }

    bool active() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    Transform& transform() { return m_transform; }
    const Transform& transform() const { return m_transform; }

    Entity* parent() const { return m_parent; }
    void setParent(Entity* parent);

    const std::vector<Entity*>& children() const { return m_children; }
    void addChild(Entity* child);

    // World-space model matrix accounting for the parent hierarchy.
    mat4 worldMatrix() const;

private:
    Id m_id;
    std::string m_name;
    bool m_active;
    Transform m_transform;
    Entity* m_parent;
    std::vector<Entity*> m_children;
};

} // namespace terra
