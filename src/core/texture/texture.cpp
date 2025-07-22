#include "core/texture/texture.hpp"
#include <stdexcept>
#include <string>
#include "glad/glad.h"
#include "stbi_image/stbi_image.h"

Core::Texture::Texture() {}

Core::Texture::Texture(int width, int height) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Core::Texture::Texture(const std::string &path, int textureLocation,
                       int colorSpace, int colorCode)
    : textureLocation(textureLocation) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(path.c_str(), &width, &height, &numberOfChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, width, height, 0, colorCode,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("failed to load texture");
  }
  stbi_image_free(data);
}

void Core::Texture::Bind() {
  glActiveTexture(textureLocation);
  glBindTexture(GL_TEXTURE_2D, id);
}

unsigned int Core::Texture::GetID() { return id; }

Core::Texture::~Texture() {}
