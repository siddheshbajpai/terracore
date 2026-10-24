#include "terra/platform/platform.h"

namespace terra {

Window::Window()
    : m_nativeHandle(nullptr)
    , m_size{0, 0}
    , m_closed(false) {}

Window::~Window() {}

bool Window::create(const std::string& title, int width, int height) {
    (void)title;
    (void)width;
    (void)height;
    m_closed = false;
    return false; // base implementation: no-op
}

void Window::pollEvents() {}

bool Window::shouldClose() const { return m_closed; }

void Window::close() { m_closed = true; }

WindowSize Window::size() const { return m_size; }

} // namespace terra
