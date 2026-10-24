#pragma once

#include <functional>
#include <vector>

#include <vulkan/vulkan.h>

#include "terra/core/math.h"

namespace terra {

class Window;

// Holds the core Vulkan objects: instance, physical device, logical device,
// queues and command pool. Does not own a swapchain.
class VulkanContext {
public:
    VulkanContext();
    ~VulkanContext();

    bool init(Window& window);

    VkInstance instance() const { return m_instance; }
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkDevice device() const { return m_device; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    VkQueue presentQueue() const { return m_presentQueue; }
    VkCommandPool commandPool() const { return m_commandPool; }
    VkSurfaceKHR surface() const { return m_surface; }
    VkSampleCountFlagBits msaaSamples() const { return m_msaaSamples; }

    uint32_t graphicsQueueFamilyIndex() const { return m_graphicsQueueFamily; }
    uint32_t presentQueueFamilyIndex() const { return m_presentQueueFamily; }

    bool isValid() const { return m_valid; }

    // One-time submit helper for initialization commands (e.g. uploads).
    VkResult oneTimeSubmit(std::function<VkCommandBuffer(VkCommandBuffer)> record);

private:
    bool pickPhysicalDevice();
    bool createLogicalDevice();
    bool createCommandPool();
    void setupDebugMessenger();

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_deviceProperties;
    VkPhysicalDeviceFeatures m_deviceFeatures;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkCommandPool m_commandPool;
    uint32_t m_graphicsQueueFamily;
    uint32_t m_presentQueueFamily;
    VkSampleCountFlagBits m_msaaSamples;
    bool m_valid;
};

} // namespace terra
