#include "core/input/mouse.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/window/window.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

Core::Mouse::Mouse(int x, int y) : x(x), y(y) {}

glm::vec2 Core::Mouse::ToNDCPosition() {
  float xPos = (float)x;
  float yPos = (float)y;

  glm::vec2 ndcMouseCoord =
      glm::vec2((xPos / (float)Window::GetWidth()) * 2.0f - 1.0f,
                (yPos / (float)Window::GetHeight()) * 2.0f - 1.0f);
  ndcMouseCoord.y = -ndcMouseCoord.y;
  return ndcMouseCoord;
}

glm::vec3 Core::Mouse::ToWorldPosition() {
  float xPos = (float)x;
  float yPos = (float)y;

  glm::vec2 ndcMouseCoord =
      glm::vec2((xPos / (float)Window::GetWidth()) * 2.0f - 1.0f,
                (yPos / (float)Window::GetHeight()) * 2.0f - 1.0f);
  ndcMouseCoord.y = -ndcMouseCoord.y;

  glm::mat4 inv = glm::inverse(Core::StaticCamera::GetProjectionMatrix() *
                               Core::StaticCamera::GetViewMatrix());

  // TODO: Maybe use glReadpixels for the depth?
  glm::vec4 wSpace = inv * glm::vec4(ndcMouseCoord, 0.0f, 1.0f);

  wSpace.w = 1.0f / wSpace.w;

  wSpace.x *= wSpace.w;
  wSpace.y *= wSpace.w;
  wSpace.z *= wSpace.w;

  return glm::vec3(wSpace.x, wSpace.y, wSpace.z);
}
