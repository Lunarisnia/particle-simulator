#include "core/texture/texture.hpp"
#include <stdexcept>
#include <string>
#include "glad/glad.h"
#include "stbi_image/stbi_image.h"

Core::Texture::Texture() {}

Core::Texture::Texture(const std::string &path) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load(path.c_str(), &width, &height, &numberOfChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("failed to load texture");
  }
  stbi_image_free(data);
}

void Core::Texture::Bind() { glBindTexture(GL_TEXTURE_2D, id); }

Core::Texture::~Texture() {}
