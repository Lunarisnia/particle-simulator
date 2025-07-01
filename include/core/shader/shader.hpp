#pragma once
#include <string>
namespace Core {
class Shader {
 private:
  std::string loadFile(const std::string &path);
  unsigned int createShader(const char *source, int shaderType);

 public:
  unsigned int id;

 public:
  void Use();

 public:
  Shader();
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();
};
}  // namespace Core
