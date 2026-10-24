#include "terra/renderer/renderer.h"

#include <iostream>

namespace terra {

Renderer::Renderer()
    : m_window(nullptr)
    , m_valid(false)
    , m_frameBegun(false) {}

Renderer::~Renderer() {}

bool Renderer::init(Window& window) {
    m_context.reset(new VulkanContext());
    if (!m_context->init(window)) {
        std::cerr << "[renderer] failed to initialize Vulkan context"
                  << std::endl;
        return false;
    }

    int w = window.size().width;
    int h = window.size().height;
    m_swapchain.reset(new Swapchain());
    if (!m_swapchain->init(*m_context, &window, w, h)) {
        std::cerr << "[renderer] failed to initialize swapchain" << std::endl;
        return false;
    }

    m_window = &window;
    m_valid = true;
    return true;
}

bool Renderer::beginFrame() {
    if (!m_valid || m_frameBegun) return false;
    m_timer.tick();
    m_frameBegun = true;
    return true;
}

void Renderer::drawFrame() {
    // Placeholder: actual command recording / submission happens here once
    // pipelines and descriptor sets are bound. For the MVP the renderer
    // presents the swapchain image directly.
    (void)m_swapchain;
}

void Renderer::endFrame() {
    if (!m_frameBegun) return;
    m_frameBegun = false;
}

void Renderer::resize(int width, int height) {
    if (!m_valid) return;
    m_swapchain->recreate(width, height);
}

float Renderer::aspectRatio() const {
    if (!m_swapchain) return 1.0f;
    VkExtent2D e = m_swapchain->extent();
    if (e.height == 0) return 1.0f;
    return (float)e.width / (float)e.height;
}

} // namespace terra
