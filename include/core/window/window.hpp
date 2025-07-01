#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Core {
class Window {
 private:
  static GLFWwindow *window;

 public:
  static void Init();
  static void Cleanup();

 private:
  static void init();
  static void createWindow();
};
};  // namespace Core
