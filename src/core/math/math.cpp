#include "core/math/math.hpp"
#include "core/vertex/vertex.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

glm::vec3 Core::Math::calculateTangent(float f, glm::vec3 edge1,
                                       glm::vec3 edge2, glm::vec2 deltaUV1,
                                       glm::vec2 deltaUV2) {
  glm::vec3 tangent;
  tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  return tangent;
}

glm::vec3 Core::Math::calculateBitangent(float f, glm::vec3 edge1,
                                         glm::vec3 edge2, glm::vec2 deltaUV1,
                                         glm::vec2 deltaUV2) {
  glm::vec3 bitangent;
  bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
  bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
  bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
  return bitangent;
}

glm::mat3 Core::Math::CreateTBNMatrix(VertexData v0, VertexData v1,
                                      VertexData v2) {
  glm::vec3 edge1 = v1.position - v0.position;
  glm::vec3 edge2 = v2.position - v0.position;
  glm::vec2 deltaUV1 = v1.textureCoord - v0.textureCoord;
  glm::vec2 deltaUV2 = v2.textureCoord - v0.textureCoord;

  float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
  glm::vec3 tangent = calculateTangent(f, edge1, edge2, deltaUV1, deltaUV2);
  glm::vec3 bitangent = calculateBitangent(f, edge1, edge2, deltaUV1, deltaUV2);

  return glm::mat3(tangent, bitangent, v0.normal);
}
