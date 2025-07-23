#pragma once
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "glad/glad.h"
namespace Core {
class Renderer {
 private:
  static std::vector<std::shared_ptr<Mesh>> renderQueue;
  // TODO: we need to create a cube without appending it to the hierarchy
  // TODO: split the procedural generation function into 2part, the procedural
  // one and the one appending it to the world
  static std::shared_ptr<Core::Object> skybox;

 public:
  static void Init();
  static void SetClearColor(float r, float g, float b, float a);
  static void Render();
  static void Clear(int bit);
  static void AdjustViewport(bool scaleUp);
  static void DepthTest(bool enable);
  static void AddToRenderQueue(std::shared_ptr<Mesh> mesh);
  static void Cleanup();
};
};  // namespace Core
