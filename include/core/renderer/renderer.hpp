#pragma once
#include "core/shader/shader.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
namespace Core {
class Renderer {
 public:
  static void Init();
  static void SetClearColor(float r, float g, float b, float a);
  static void Render();
  static void Clear();
  static void Cleanup();

 private:
  static unsigned int shaderProgram, VAO, VBO, EBO;
  static Shader *program;
  static void triangle();
  static void renderTriangle();
};
};  // namespace Core
