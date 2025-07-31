#include "core/static_light/static_light.hpp"
#include <memory>
#include "core/components/transform.hpp"
#include "core/window/window.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
std::shared_ptr<Core::Transform> Core::StaticLight::transform;
glm::vec3 Core::StaticLight::front = glm::vec3(0.0f, 0.0f, -1.0f);

void Core::StaticLight::Init() {
  transform = std::make_shared<Core::Transform>();
  transform->position.z = 2.0f;
  transform->position.x = 1.0f;
  transform->position.y = 4.0f;
}

glm::mat4 Core::StaticLight::GetViewMatrix() {
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::mat4 view = glm::lookAt(transform->position, glm::vec3(0.0f), up);

  return view;
}

glm::mat4 Core::StaticLight::GetProjectionMatrix() {
  float near_plane = 1.0f, far_plane = 7.5f;
  glm::mat4 lightProjection =
      glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

  return lightProjection;
}

glm::mat4 Core::StaticLight::GetLightSpaceMatrix() {
  return GetProjectionMatrix() * GetViewMatrix();
}

void Core::StaticLight::Cleanup() {}
