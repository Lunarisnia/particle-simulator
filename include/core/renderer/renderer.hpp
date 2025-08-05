#pragma once
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/shader/shader.hpp"
#include "glad/glad.h"
namespace Core {
class Renderer {
 private:
  static std::vector<std::shared_ptr<Mesh>> renderQueue;
  static std::shared_ptr<Core::Object> skybox;
  static Shader shadowMappingShader;

 public:
  static void Init();
  static void SetClearColor(float r, float g, float b, float a);
  static void Render();
  static void RenderShadowMap();
  static void RenderShadowCubeMap();
  static void Clear(int bit);
  static void AdjustViewport(int width, int height, bool scaleUp);
  static void DepthTest(bool enable);
  static void AddToRenderQueue(std::shared_ptr<Mesh> mesh);
  static void Cleanup();
};
};  // namespace Core
