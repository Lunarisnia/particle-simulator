#include "core/texture/texture.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include "core/texture/texture_manager.hpp"
#include "stbi_image/stbi_image.h"

Core::Texture::Texture() {}

Core::Texture::Texture(int width, int height)
    : textureLocation(TextureManager::ScreenTextureLocation()),
      textureType(GL_TEXTURE_2D) {
  glGenTextures(1, &id);
  glBindTexture(textureType, id);

  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(textureType, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(textureType, 0);
}

Core::Texture::Texture(const std::string &path, int textureLocation,
                       int colorSpace, int colorCode)
    : textureLocation(textureLocation), textureType(GL_TEXTURE_2D) {
  glGenTextures(1, &id);
  glBindTexture(textureType, id);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(path.c_str(), &width, &height, &numberOfChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, width, height, 0, colorCode,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(textureType);
  } else {
    throw std::runtime_error("failed to load texture");
  }
  stbi_image_free(data);
}

Core::Texture::Texture(std::map<TextureTarget, std::string> textureFaces,
                       int textureLocation, int colorSpace, int colorCode)
    : textureLocation(textureLocation), textureType(GL_TEXTURE_CUBE_MAP) {
  glGenTextures(1, &id);
  glBindTexture(textureType, id);
  stbi_set_flip_vertically_on_load(false);
  for (auto &pair : textureFaces) {
    data = stbi_load(pair.second.c_str(), &width, &height, &numberOfChannel, 0);
    if (data) {
      glTexImage2D(pair.first, 0, colorSpace, width, height, 0, colorCode,
                   GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      stbi_image_free(data);
      throw std::runtime_error("failed to load texture");
    }
  }

  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Core::Texture::Bind() {
  glActiveTexture(textureLocation);
  glBindTexture(textureType, id);
}

unsigned int Core::Texture::GetID() { return id; }

Core::Texture::~Texture() {}
