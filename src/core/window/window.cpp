#include "core/window/window.hpp"
#include <stdexcept>
#include "GLFW/glfw3.h"

int Core::Window::width = 1280;
int Core::Window::height = 768;
GLFWwindow *Core::Window::window;

void Core::Window::Init() {
  initGLFW();
  createWindow();
}

int Core::Window::GetHeight(bool retina) {
  if (retina) {
    return height * 2.0f;
  }
  return height;
}
int Core::Window::GetWidth(bool retina) {
  if (retina) {
    return width * 2.0f;
  }
  return width;
}

bool Core::Window::ShouldClose() {
  if (window == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  return glfwWindowShouldClose(window);
}

void Core::Window::PollEvent() { glfwPollEvents(); }

void Core::Window::SwapBuffer() {
  if (window == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  glfwSwapBuffers(window);
}

GLFWwindow *Core::Window::GetWindow() { return window; }

void Core::Window::initGLFW() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initiate glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Core::Window::createWindow() {
  window =
      glfwCreateWindow(width, height, "Particle Renderer", nullptr, nullptr);
  if (window == nullptr) {
    throw std::runtime_error("failed to create glfw window");
  }
  glfwMakeContextCurrent(window);
}

void Core::Window::Cleanup() {
  if (window != nullptr) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}
