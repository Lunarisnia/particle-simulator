#include "core/static_camera/static_camera.hpp"
#include <iostream>
#include <memory>
#include "core/components/transform.hpp"
#include "core/window/window.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"

std::shared_ptr<Core::Transform> Core::StaticCamera::transform;

void Core::StaticCamera::Init() { transform = std::make_shared<Transform>(); }

glm::mat4 Core::StaticCamera::GetViewMatrix() {
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::mat4 view =
      glm::lookAt(transform->position, transform->position + front, up);

  return view;
}

glm::mat4 Core::StaticCamera::GetProjectionMatrix() {
  glm::mat4 projection = glm::perspective(
      glm::radians(90.0f),
      (float)Window::GetWidth() / (float)Window::GetHeight(), 0.01f, 100.0f);

  return projection;
}

void Core::StaticCamera::Cleanup() {}
