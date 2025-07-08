#include "core/shader/shader.hpp"
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

Core::Shader::Shader() {}

Core::Shader::Shader(const std::string &vertexPath,
                     const std::string &fragmentPath) {
  int success;
  char infoLog[512];

  std::string vertexCode = loadFile(vertexPath);
  std::string fragmentCode = loadFile(fragmentPath);

  unsigned int vertexShader =
      createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
  unsigned int fragmentShader =
      createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    throw std::runtime_error(infoLog);
  }

  glDeleteProgram(vertexShader);
  glDeleteProgram(fragmentShader);
}

void Core::Shader::Use() { glUseProgram(id); }

void Core::Shader::SetMat4(const std::string &uniform, glm::mat4 mat4) {
  Use();
  int uniformId = glGetUniformLocation(id, uniform.c_str());
  glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Core::Shader::SetVec3(const std::string &uniform, glm::vec3 vec3) {
  Use();
  int uniformId = glGetUniformLocation(id, uniform.c_str());
  glUniform3fv(uniformId, 1, glm::value_ptr(vec3));
}

unsigned int Core::Shader::createShader(const char *source, int shaderType) {
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  return shader;
}

std::string Core::Shader::loadFile(const std::string &path) {
  std::ifstream file(path.c_str());
  std::string code;

  std::stringstream stream;
  stream << file.rdbuf();
  code = stream.str();

  file.close();

  return code;
}

Core::Shader::~Shader() {}
