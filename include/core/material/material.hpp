#pragma once
#include <string>
#include <vector>
#include "core/shader/shader.hpp"
#include "core/texture/texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Material {
 public:
  Shader shader;
  std::vector<Texture> textures;

 public:
  void Use();
  void SetMat4(const std::string &uniform, glm::mat4 mat4);
  void SetVec3(const std::string &uniform, glm::vec3 vec3);
  void SetInt(const std::string &uniform, int i);
  void SetFloat(const std::string &uniform, float f);

  void AddTexture(Texture texture);

  void LoadTexture(const std::string &path, int colorSpace, int colorCode);

 public:
  Material(Shader shader);
};
}  // namespace Core
