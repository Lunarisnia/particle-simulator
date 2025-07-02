#pragma once
#include "core/components/component.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class Transform : public Component {
 public:
  glm::vec3 position;

 public:
  void Update() override;
  ComponentType GetType() override;
  glm::mat4 GetTransformMatrix();
};
}  // namespace Core
