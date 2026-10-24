#pragma once

#include <string>

namespace terra {

// Platform abstraction: window creation and input event pumping.
// The GLFW backend lives in platform/glfw_window.cpp and is enabled via
// TERRA_CORE_USE_GLFW in CMake. This header intentionally has no Vulkan
// dependency so that terra_core stays renderer-agnostic.

struct WindowSize {
    int width;
    int height;
};

class Window {
public:
    Window();
    virtual ~Window();

    // Create the window. Returns false on failure.
    virtual bool create(const std::string& title, int width, int height);

    // Poll events / pump the message loop.
    virtual void pollEvents();

    // Returns true while the window should remain open.
    virtual bool shouldClose() const;

    // Request window closure.
    void close();

    WindowSize size() const;

    void* nativeHandle() const { return m_nativeHandle; }

protected:
    void* m_nativeHandle;
    WindowSize m_size;
    bool m_closed;
};

} // namespace terra
