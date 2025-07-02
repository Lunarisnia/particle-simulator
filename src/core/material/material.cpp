#include "core/material/material.hpp"
#include <string>
#include "core/shader/shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"

Core::Material::Material(Core::Shader shader) : shader(shader) {}

void Core::Material::Use() { shader.Use(); }

void Core::Material::SetMat4(const std::string &uniform, glm::mat4 mat4) {
  shader.SetMat4(uniform, mat4);
}
