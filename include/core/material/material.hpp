#pragma once
#include <string>
#include "core/shader/shader.hpp"
#include "core/texture/texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Material {
 public:
  Shader shader;
  Texture texture;

 public:
  void Use();
  void SetMat4(const std::string &uniform, glm::mat4 mat4);
  void SetVec3(const std::string &uniform, glm::vec3 vec3);
  void SetFloat(const std::string &uniform, float f);

  void LoadTexture(const std::string &path);

 public:
  Material(Shader shader);
};
}  // namespace Core
