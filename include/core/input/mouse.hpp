#pragma once
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class Mouse {
 public:
  glm::vec2 position;
  int x, y;

 public:
  glm::vec3 ToWorldPosition();
  glm::vec2 ToNDCPosition();

 public:
  Mouse(int x, int y);
};
};  // namespace Core
