#pragma once
#include <string>
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Shader {
 private:
  std::string loadFile(const std::string &path);
  unsigned int createShader(const char *source, int shaderType);

 public:
  unsigned int id;

 public:
  void Use();
  void SetMat4(const std::string &uniform, glm::mat4 mat4);

 public:
  Shader();
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();
};
}  // namespace Core
