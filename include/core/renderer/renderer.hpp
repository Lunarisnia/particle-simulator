#pragma once
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "glad/glad.h"
namespace Core {
class Renderer {
 private:
  static std::vector<std::shared_ptr<Mesh>> renderQueue;

 public:
  static void Init();
  static void SetClearColor(float r, float g, float b, float a);
  static void Render();
  static void Clear();
  static void AddToRenderQueue(std::shared_ptr<Mesh> mesh);
  static void Cleanup();
};
};  // namespace Core
