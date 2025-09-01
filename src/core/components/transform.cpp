#include "core/components/transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

void Core::Transform::Update() {}

glm::mat4 Core::Transform::GetTransformMatrix() {
  glm::mat4 model(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, scale);
  // TODO: Implement as less janky version of this
  model = glm::rotate(model, glm::radians(-90.0f),
                      glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

  return model;
}

glm::vec3 Core::Transform::GetWorldPosition() {
  /*return GetTransformMatrix() * glm::vec4(position, 1.0f);*/
  return position;
}

Core::ComponentType Core::Transform::GetType() {
  return ComponentType::TRANSFORM;
}
