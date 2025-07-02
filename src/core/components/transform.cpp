#include "core/components/transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

void Core::Transform::Update() {}

glm::mat4 Core::Transform::GetTransformMatrix() {
  glm::mat4 model(1.0f);
  model = glm::translate(model, position);

  return model;
}

Core::ComponentType Core::Transform::GetType() {
  return ComponentType::TRANSFORM;
}
