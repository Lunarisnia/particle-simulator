#include "core/texture/texture_manager.hpp"
#include "glad/glad.h"
#include <map>
#include <string>
#include "core/texture/texture.hpp"

int Core::TextureManager::nextAvailableTextureId = GL_TEXTURE0;
std::map<std::string, Core::Texture> Core::TextureManager::loadedTextures;

Core::Texture Core::TextureManager::LoadTexture(const std::string &path,
                                                int colorSpace, int colorCode) {
  if (loadedTextures.count(path) > 0) {
    return loadedTextures.at(path);
  }

  std::string fileType =
      path.substr(path.find_last_of(".") + 1, std::string::npos);
  Core::Texture texture{path, nextAvailableTextureId++, colorSpace, colorCode};
  loadedTextures.emplace(path, texture);

  return texture;
}
