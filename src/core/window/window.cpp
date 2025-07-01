#include "core/window/window.hpp"
#include <stdexcept>
#include "GLFW/glfw3.h"

GLFWwindow *Core::Window::window;

void Core::Window::Init() {
  init();
  createWindow();
}

void Core::Window::init() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initiate glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Core::Window::createWindow() {
  window = glfwCreateWindow(800, 600, "Particle Renderer", nullptr, nullptr);
  if (window == nullptr) {
    throw std::runtime_error("failed to create glfw window");
  }
}

void Core::Window::Cleanup() { glfwTerminate(); }
