#pragma once
#include <memory>
#include "core/components/transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class StaticLight {
 public:
  static std::shared_ptr<Core::Transform> transform;
  static glm::vec3 front;

 public:
  static void Init();
  static glm::mat4 GetViewMatrix();
  static glm::mat4 GetProjectionMatrix();
  static glm::mat4 GetLightSpaceMatrix();
  static void Cleanup();
};
};  // namespace Core
