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
  static Shader CreateShader(const std::string &vertPath,
                             const std::string &fragPath);
  static Shader CreateShaderWithGeometry(const std::string &vertPath,
                                         const std::string &geometryPath,
                                         const std::string &fragPath);

 public:
  void Use();
  void SetMat4(const std::string &uniform, glm::mat4 mat4);
  void SetVec3(const std::string &uniform, glm::vec3 vec3);
  void SetInt(const std::string &uniform, int i);
  void SetFloat(const std::string &uniform, float f);

 public:
  Shader();
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();
};
}  // namespace Core
