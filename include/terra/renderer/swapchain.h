#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "terra/core/math.h"

namespace terra {

class VulkanContext;

// Manages the Vulkan swapchain, its images, image views and the render pass
// used for on-screen rendering.
class Swapchain {
public:
    Swapchain();
    ~Swapchain();

    bool init(VulkanContext& context, Window* window, int width, int height);
    void shutdown();

    // Recreate after a resize. Returns false if the surface is still lost.
    bool recreate(int width, int height);

    VkRenderPass renderPass() const { return m_renderPass; }
    VkSwapchainKHR handle() const { return m_swapchain; }
    VkExtent2D extent() const { return m_extent; }
    VkFormat imageFormat() const { return m_imageFormat; }
    const std::vector<VkImage>& images() const { return m_images; }
    const std::vector<VkImageView>& imageViews() const { return m_imageViews; }
    const std::vector<VkFramebuffer>& framebuffers() const { return m_framebuffers; }
    size_t imageCount() const { return m_images.size(); }

    bool isValid() const { return m_valid; }

private:
    VulkanContext* m_context;
    Window* m_window;
    VkSwapchainKHR m_swapchain;
    VkFormat m_imageFormat;
    VkExtent2D m_extent;
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
    VkRenderPass m_renderPass;
    std::vector<VkFramebuffer> m_framebuffers;
    VkDevice m_device;
    bool m_valid;
};

} // namespace terra
