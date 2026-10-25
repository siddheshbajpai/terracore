#include "terra/scene/entity.h"

#include <algorithm>

namespace terra {

Entity::Entity(Id id, std::string name)
    : m_id(id)
    , m_name(std::move(name))
    , m_active(true)
    , m_parent(nullptr) {}

void Entity::setParent(Entity* parent) {
    // Detach from current parent.
    if (m_parent) {
        auto& siblings = m_parent->m_children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this),
                       siblings.end());
    }
    m_parent = parent;
    if (parent) {
        parent->m_children.push_back(this);
    }
}

void Entity::addChild(Entity* child) {
    if (!child) return;
    child->setParent(this);
}

mat4 Entity::worldMatrix() const {
    mat4 result = transform().localMatrix();
    const Entity* p = m_parent;
    while (p) {
        result = p->transform().localMatrix() * result;
        p = p->m_parent;
    }
    return result;
}

} // namespace terra
