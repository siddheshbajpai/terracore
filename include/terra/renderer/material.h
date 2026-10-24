#pragma once

#include <vulkan/vulkan.h>

#include "terra/core/math.h"

namespace terra {

class VulkanContext;

// Basic material: a texture reference plus a tint color. PBR is deferred to a
// later milestone.
class Material {
public:
    Material();
    ~Material();

    bool init(VulkanContext& context);

    void shutdown();

    void setTexture(Texture* texture) { m_texture = texture; }
    Texture* texture() const { return m_texture; }
    void setTint(const vec3& tint) { m_tint = tint; }
    const vec3& tint() const { return m_tint; }

    bool isValid() const { return m_valid; }

private:
    VkDevice m_device;
    Texture* m_texture;
    vec3 m_tint;
    bool m_valid;
};

} // namespace terra
