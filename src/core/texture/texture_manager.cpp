#include "core/texture/texture_manager.hpp"
#include "glad/glad.h"
#include <map>
#include <string>
#include "core/texture/texture.hpp"

int Core::TextureManager::nextAvailableTextureId = GL_TEXTURE0 + 1;
std::map<std::string, Core::Texture> Core::TextureManager::loadedTextures;

int Core::TextureManager::ScreenTextureLocation() { return GL_TEXTURE0; }

int Core::TextureManager::GetTextureLocation(const std::string &path) {
  if (loadedTextures.count(path) < 1) {
    return -1;
  }
  int number = loadedTextures.at(path).GetLocation();
  return number - GL_TEXTURE0;
}

Core::Texture Core::TextureManager::CreateTexture(
    const std::string &name, int width, int height, int textureType,
    int colorSpace, int colorCode, int numberFormat, bool emptyParam) {
  if (loadedTextures.count(name) > 0) {
    return loadedTextures.at(name);
  }
  Core::Texture texture{width,        height,     nextAvailableTextureId++,
                        textureType,  colorSpace, colorCode,
                        numberFormat, emptyParam};
  loadedTextures.emplace(name, texture);

  return texture;
}

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

Core::Texture Core::TextureManager::LoadTextureCubeMap(
    std::map<Core::TextureTarget, std::string> textureFaces, int colorSpace,
    int colorCode) {
  Core::Texture texture{textureFaces, nextAvailableTextureId++, colorSpace,
                        colorCode};
  return texture;
}
