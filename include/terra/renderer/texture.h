#pragma once

#include <string>

#include <vulkan/vulkan.h>

namespace terra {

class VulkanContext;

// A GPU texture with an associated sampler and image view.
class Texture {
public:
    Texture();
    ~Texture();

    // Load an RGBA8 image from raw pixels and create device-local resources.
    bool createFromPixels(VulkanContext& context,
                          const uint8_t* pixels, int width, int height,
                          int channels);

    void shutdown();

    VkImageView view() const { return m_imageView; }
    VkSampler sampler() const { return m_sampler; }

    bool isValid() const { return m_valid; }

private:
    VkDevice m_device;
    VkImage m_image;
    VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
    VkSampler m_sampler;
    bool m_valid;
};

} // namespace terra
