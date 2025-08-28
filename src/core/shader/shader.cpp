#include "core/shader/shader.hpp"
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>

Core::Shader Core::Shader::CreateShader(const std::string &vertPath,
                                        const std::string &fragPath) {
  Shader shader;
  int success;
  char infoLog[512];

  std::string vertexCode = shader.loadFile(vertPath);
  std::string fragmentCode = shader.loadFile(fragPath);

  unsigned int vertexShader =
      shader.createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
  unsigned int fragmentShader =
      shader.createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

  shader.id = glCreateProgram();
  glAttachShader(shader.id, vertexShader);
  glAttachShader(shader.id, fragmentShader);
  glLinkProgram(shader.id);

  glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
    std::println("{}", infoLog);
    throw std::runtime_error(infoLog);
  }

  glDeleteProgram(vertexShader);
  glDeleteProgram(fragmentShader);
  return shader;
}

Core::Shader Core::Shader::CreateShaderWithGeometry(
    const std::string &vertPath, const std::string &geometryPath,
    const std::string &fragPath) {
  Shader shader;
  int success;
  char infoLog[512];

  std::string vertexCode = shader.loadFile(vertPath);
  std::string fragmentCode = shader.loadFile(fragPath);
  std::string geometryCode = shader.loadFile(geometryPath);

  unsigned int vertexShader =
      shader.createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::println("Vertex: {}", infoLog);
    throw std::runtime_error(infoLog);
  }
  unsigned int geometryShader =
      shader.createShader(geometryCode.c_str(), GL_GEOMETRY_SHADER);
  glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
    std::println("Geometry: {}", infoLog);
    throw std::runtime_error(infoLog);
  }
  unsigned int fragmentShader =
      shader.createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::println("Fragment: {}", infoLog);
    throw std::runtime_error(infoLog);
  }

  shader.id = glCreateProgram();
  glAttachShader(shader.id, vertexShader);
  glAttachShader(shader.id, geometryShader);
  glAttachShader(shader.id, fragmentShader);
  glLinkProgram(shader.id);

  glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader.id, 512, NULL, infoLog);
    std::println("{}", infoLog);
    throw std::runtime_error(infoLog);
  }

  glDeleteProgram(vertexShader);
  glDeleteProgram(geometryShader);
  glDeleteProgram(fragmentShader);
  return shader;
}

Core::Shader::Shader() {}

Core::Shader::Shader(const std::string &vertexPath,
                     const std::string &fragmentPath) {
  int success;
  char infoLog[512];

  std::string vertexCode = loadFile(vertexPath);
  std::string fragmentCode = loadFile(fragmentPath);

  unsigned int vertexShader =
      createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::println("Vertex: {}", infoLog);
    throw std::runtime_error(infoLog);
  }
  unsigned int fragmentShader =
      createShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::println("Fragment: {}", infoLog);
    throw std::runtime_error(infoLog);
  }

  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    std::println("{}", infoLog);
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

void Core::Shader::SetInt(const std::string &uniform, int i) {
  Use();
  int uniformId = glGetUniformLocation(id, uniform.c_str());
  glUniform1i(uniformId, i);
}

void Core::Shader::SetFloat(const std::string &uniform, float f) {
  Use();
  int uniformId = glGetUniformLocation(id, uniform.c_str());
  glUniform1f(uniformId, f);
}

void Core::Shader::SetVec2(const std::string &uniform, glm::vec2 vec2) {
  Use();
  int uniformId = glGetUniformLocation(id, uniform.c_str());
  glUniform2fv(uniformId, 1, glm::value_ptr(vec2));
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
