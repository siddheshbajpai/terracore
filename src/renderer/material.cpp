#include "terra/renderer/material.h"

#include "terra/renderer/texture.h"

namespace terra {

Material::Material()
    : m_device(VK_NULL_HANDLE)
    , m_texture(nullptr)
    , m_tint(1.0f, 1.0f, 1.0f)
    , m_valid(false) {}

Material::~Material() { shutdown(); }

bool Material::init(VulkanContext& context) {
    m_device = context.device();
    m_valid = true;
    return true;
}

void Material::shutdown() {
    m_device = VK_NULL_HANDLE;
    m_texture = nullptr;
    m_valid = false;
}

} // namespace terra
