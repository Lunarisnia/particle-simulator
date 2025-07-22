#pragma once
#include <map>
#include <string>
#include "core/texture/texture.hpp"
namespace Core {
class TextureManager {
 private:
  static int nextAvailableTextureId;
  static std::map<std::string, Core::Texture> loadedTextures;

 public:
  static Texture LoadTexture(const std::string &path, int colorSpace,
                             int colorCode);
};
};  // namespace Core
