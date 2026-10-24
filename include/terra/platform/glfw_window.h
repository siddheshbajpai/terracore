#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "terra/platform/platform.h"

namespace terra {

// GLFW window backend. Enabled via TERRA_CORE_USE_GLFW in CMake.
class GlfwWindow : public Window {
public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual bool create(const std::string& title, int width, int height) override;
    virtual void pollEvents() override;
    virtual bool shouldClose() const override;

    // Vulkan surface helpers. `handle` is the native window handle
    // (GLFWwindow* on the GLFW backend).
    static void getRequiredExtensions(std::vector<const char*>& extensions);
    static bool createSurface(VkInstance instance, void* handle,
                              VkSurfaceKHR* surface);

private:
    GLFWwindow* m_handle;
};

} // namespace terra
