#include "terra/platform/platform.h"

#include <vector>
#include <vulkan/vulkan.h>

#if defined(TERRA_CORE_USE_GLFW) && TERRA_CORE_USE_GLFW
#    include <GLFW/glfw3.h>
#endif

#include <cstring>
#include <stdexcept>

namespace terra {

class GlfwWindow : public Window {
public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual bool create(const std::string& title, int width, int height) override;
    virtual void pollEvents() override;
    virtual bool shouldClose() const override;

    // Vulkan surface helpers (declared here because this backend owns GLFW).
    static void getRequiredExtensions(std::vector<const char*>& extensions);
    static bool createSurface(VkInstance instance, GLFWwindow* handle,
                              VkSurfaceKHR* surface);

private:
    GLFWwindow* m_handle;
};

} // namespace terra

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

#if defined(TERRA_CORE_USE_GLFW) && TERRA_CORE_USE_GLFW

namespace terra {

GlfwWindow::GlfwWindow() : m_handle(nullptr) {}

GlfwWindow::~GlfwWindow() {
    if (m_handle) {
        glfwDestroyWindow(m_handle);
        m_handle = nullptr;
    }
}

bool GlfwWindow::create(const std::string& title, int width, int height) {
    if (!glfwInit()) {
        return false;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_handle) {
        return false;
    }
    m_size.width = width;
    m_size.height = height;
    m_closed = false;
    return true;
}

void GlfwWindow::pollEvents() {
    glfwPollEvents();
}

bool GlfwWindow::shouldClose() const {
    return m_handle ? glfwWindowShouldClose(m_handle) : true;
}

void GlfwWindow::getRequiredExtensions(std::vector<const char*>& extensions) {
    uint32_t count = 0;
    const char** names = glfwGetRequiredInstanceExtensions(&count);
    extensions.assign(names, names + count);
}

bool GlfwWindow::createSurface(VkInstance instance, void* handle,
                               VkSurfaceKHR* surface) {
    VkResult res =
        glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(handle),
                                nullptr, surface);
    return res == VK_SUCCESS;
}

} // namespace terra

#else // TERRA_CORE_USE_GLFW not set

namespace terra {

GlfwWindow::GlfwWindow() {}
GlfwWindow::~GlfwWindow() {}
bool GlfwWindow::create(const std::string&, int, int) { return false; }
void GlfwWindow::pollEvents() {}
bool GlfwWindow::shouldClose() const { return true; }

void GlfwWindow::getRequiredExtensions(std::vector<const char*>&) {}
bool GlfwWindow::createSurface(VkInstance, void*, VkSurfaceKHR*) { return false; }

} // namespace terra

#endif
