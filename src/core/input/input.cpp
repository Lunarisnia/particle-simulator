#include "core/input/input.hpp"
#include <stdexcept>
#include "GLFW/glfw3.h"
#include "core/input/mouse.hpp"
#include "core/window/window.hpp"
#include "glm/ext/vector_float2.hpp"

Core::Mouse Core::Input::mouse(0, 0);

void Core::Input::Init() {
  if (Window::GetWindow() == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetInputMode(Window::GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
}

bool Core::Input::GetKey(int key) {
  if (Window::GetWindow() == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  return glfwGetKey(Window::GetWindow(), key) == GLFW_PRESS;
}

bool Core::Input::GetMouseButton(int key) {
  if (Window::GetWindow() == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  return glfwGetMouseButton(Window::GetWindow(), key) == GLFW_PRESS;
}

void Core::Input::PollMouse() {
  if (Window::GetWindow() == nullptr) {
    throw std::runtime_error("window is invalid");
  }
  double xPos, yPos;
  glfwGetCursorPos(Window::GetWindow(), &xPos, &yPos);

  mouse.x = xPos;
  mouse.y = yPos;
  mouse.position = glm::vec2(xPos, yPos);
}

void Core::Input::Cleanup() {}
