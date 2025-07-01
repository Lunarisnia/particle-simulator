#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
namespace Core {
class Renderer {
 public:
  static void Init();
  static void Cleanup();
};
};  // namespace Core
