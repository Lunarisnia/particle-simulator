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
  static int ScreenTextureLocation();
  static int GetTextureLocation(const std::string &path);
  static Texture CreateTexture(const std::string &name, int width, int height);
  static Texture LoadTexture(const std::string &path, int colorSpace,
                             int colorCode);
  static Texture LoadTextureCubeMap(
      std::map<Core::TextureTarget, std::string> textureFaces, int colorSpace,
      int colorCode);
};
};  // namespace Core
