#pragma once
#include <memory>
#include "core/components/transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class StaticCamera {
 public:
  static std::shared_ptr<Transform> transform;

 public:
  static void Init();
  static glm::mat4 GetViewMatrix();
  static glm::mat4 GetProjectionMatrix();
  static void Cleanup();
};
}  // namespace Core
