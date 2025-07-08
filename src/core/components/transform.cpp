#include "core/components/transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

void Core::Transform::Update() {}

glm::mat4 Core::Transform::GetTransformMatrix() {
  glm::mat4 model(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, scale);
  /*model = glm::rotate(model, glm::radians(Time::timeSinceStartup) * 5.0f,*/
  /*                    glm::vec3(0.5f, 0.5f, 0.0f));*/

  return model;
}

Core::ComponentType Core::Transform::GetType() {
  return ComponentType::TRANSFORM;
}
