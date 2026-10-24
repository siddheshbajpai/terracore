#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "terra/core/math.h"

namespace terra {

class VulkanContext;
class Swapchain;

// Encapsulates a graphics pipeline, its layout and descriptor set layout.
class Pipeline {
public:
    Pipeline();
    ~Pipeline();

    // Build a simple shaded pipeline (vertex + fragment) from SPIR-V source
    // files. Returns false on failure.
    bool init(VulkanContext& context, Swapchain& swapchain,
              const std::string& vertSpv, const std::string& fragSpv);

    void shutdown();

    VkPipeline handle() const { return m_pipeline; }
    VkPipelineLayout layout() const { return m_pipelineLayout; }
    VkDescriptorSetLayout descriptorSetLayout() const { return m_descriptorSetLayout; }
    VkRenderPass renderPass() const { return m_renderPass; }

    bool isValid() const { return m_valid; }

private:
    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkDevice m_device;
    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkRenderPass m_renderPass;
    bool m_valid;
};

} // namespace terra
