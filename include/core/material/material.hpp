#pragma once
#include <string>
#include "core/shader/shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Material {
 public:
  Shader shader;

 public:
  void Use();
  void SetMat4(const std::string &uniform, glm::mat4 mat4);

 public:
  Material(Shader shader);
};
}  // namespace Core
