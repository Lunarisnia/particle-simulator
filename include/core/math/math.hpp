#pragma once
#include "core/vertex/vertex.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class Math {
 private:
  static glm::vec3 calculateTangent(float f, glm::vec3 edge1, glm::vec3 edge2,
                                    glm::vec2 deltaUV1, glm::vec2 deltaUV2);
  static glm::vec3 calculateBitangent(float f, glm::vec3 edge1, glm::vec3 edge2,
                                      glm::vec2 deltaUV1, glm::vec2 deltaUV2);

 public:
  static glm::mat3 CreateTBNMatrix(VertexData v0, VertexData v1, VertexData v2);
};
};  // namespace Core
