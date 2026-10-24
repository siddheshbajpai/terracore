#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include "terra/core/math.h"
#include "terra/core/timer.h"
#include "terra/platform/platform.h"

namespace terra {

class VulkanContext;
class Swapchain;
class Pipeline;
class Mesh;
class Texture;
class CommandBuffer;
class Profiler;

// High-level renderer interface. Owns the Vulkan context, swapchain and the
// main draw loop. Conventional (non-voxel) rendering goes through here.
class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialize Vulkan using the given window. Returns false on failure.
    bool init(Window& window);

    // Begin a frame. Returns false if the frame should be skipped (e.g.
    // window resized / out of date).
    bool beginFrame();

    // Record and submit draw commands for the current frame.
    void drawFrame();

    // Present the current frame and end it.
    void endFrame();

    // Recreate the swapchain (called on window resize).
    void resize(int width, int height);

    bool isValid() const { return m_valid; }
    VulkanContext& context() { return *m_context; }
    const VulkanContext& context() const { return *m_context; }
    Swapchain& swapchain() { return *m_swapchain; }

    float aspectRatio() const;

private:
    std::unique_ptr<VulkanContext> m_context;
    std::unique_ptr<Swapchain> m_swapchain;
    std::unique_ptr<Pipeline> m_pipeline;
    Window* m_window;
    Timer m_timer;
    bool m_valid;
    bool m_frameBegun;
};

} // namespace terra
