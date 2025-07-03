#pragma once
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class VertexData {
  float x;
  float y;
  float z;
  float u;
  float v;

 public:
  inline void SetPosition(glm::vec3 position) {
    x = position.x;
    y = position.y;
    z = position.z;
  }
  inline void SetTextureCoordinate(glm::vec2 texCoord) {
    u = texCoord.x;
    v = texCoord.y;
  }

 public:
  inline VertexData(float x, float y, float z) : x(x), y(y), z(z) {};
  inline VertexData(float x, float y, float z, float u, float v)
      : x(x), y(y), z(z), u(u), v(v) {};
  inline VertexData() {};
};
};  // namespace Core
