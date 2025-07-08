#pragma once
#include "core/components/component.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class Transform : public Component {
 public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale = glm::vec3(1.0f);

 public:
  void Update() override;
  ComponentType GetType() override;
  glm::mat4 GetTransformMatrix();
};
}  // namespace Core
