#pragma once
#include "GLFW/glfw3.h"

namespace Core {
class Window {
 private:
  static int width, height;
  static GLFWwindow *window;

 public:
  static void Init();
  static bool ShouldClose();
  static void PollEvent();
  static void SwapBuffer();
  static void Cleanup();
  static GLFWwindow *GetWindow();
  static int GetWidth();
  static int GetHeight();

 private:
  static void initGLFW();
  static void createWindow();
};
};  // namespace Core
