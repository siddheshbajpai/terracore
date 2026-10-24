#pragma once

#include <vulkan/vulkan.h>

namespace terra {

class VulkanContext;

// Thin wrapper around a primary Vulkan command buffer for the MVP.
class CommandBuffer {
public:
    CommandBuffer();
    ~CommandBuffer();

    bool allocate(VulkanContext& context,
                  VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    void free();

    VkCommandBuffer handle() const { return m_commandBuffer; }

    bool begin(VkCommandBufferUsageFlags flags = 0);
    bool end();
    void reset();

    bool isValid() const { return m_valid; }

private:
    VulkanContext* m_context;
    VkCommandBuffer m_commandBuffer;
    bool m_valid;
};

} // namespace terra
