#include "core/material/material.hpp"
#include <string>
#include "glad/glad.h"
#include "core/shader/shader.hpp"
#include "core/texture/texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"

Core::Material::Material(Core::Shader shader) : shader(shader) {}

void Core::Material::Use() {
  shader.Use();
  for (Texture &texture : textures) {
    texture.Bind();
  }
}

void Core::Material::SetMat4(const std::string &uniform, glm::mat4 mat4) {
  shader.SetMat4(uniform, mat4);
}

void Core::Material::SetVec3(const std::string &uniform, glm::vec3 vec3) {
  shader.SetVec3(uniform, vec3);
}

void Core::Material::SetInt(const std::string &uniform, int i) {
  shader.SetInt(uniform, i);
}

void Core::Material::SetFloat(const std::string &uniform, float f) {
  shader.SetFloat(uniform, f);
}

void Core::Material::LoadTexture(const std::string &path,
                                 unsigned int textureLocation, int colorCode) {
  textures.emplace_back(Texture{path, textureLocation, colorCode});
}

void Core::Material::AddTexture(Texture texture) {
  textures.emplace_back(texture);
}
