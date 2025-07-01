#include "core/renderer/renderer.hpp"
#include <stdexcept>
#include "core/window/window.hpp"

void Core::Renderer::Init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("failed to initiate GLAD");
  }

#if defined(WIN32)
  glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
#else
  glViewport(0, 0, Window::GetWidth() * 2, Window::GetHeight() * 2);
#endif
}

void Core::Renderer::Cleanup() {}
