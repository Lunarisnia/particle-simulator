#pragma once
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class VertexData {
 public:
  glm::vec3 position;
  glm::vec2 textureCoord;
  glm::vec3 normal;
  glm::vec3 tangent;

 public:
  inline void SetPosition(glm::vec3 p) { position = p; }
  inline void SetTextureCoordinate(glm::vec2 texCoord) {
    textureCoord = texCoord;
  }
  inline void SetNormal(glm::vec3 n) { normal = n; }
  inline void SetTangent(glm::vec3 t) { tangent = t; }

 public:
  inline VertexData(float x, float y, float z) : position(x, y, z) {};
  inline VertexData(float x, float y, float z, float u, float v)
      : position(x, y, z), textureCoord(u, v) {};
  inline VertexData(glm::vec3 p, glm::vec2 texCoord, glm::vec3 n)
      : position(p), textureCoord(texCoord), normal(n) {};
  inline VertexData() {};
};
};  // namespace Core
