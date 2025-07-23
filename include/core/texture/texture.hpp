#pragma once
#include "glad/glad.h"
#include <map>
#include <string>
namespace Core {
enum TextureTarget {
  CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};
class Texture {
 private:
  unsigned int id;
  int textureType;
  int textureLocation;

 public:
  int width, height, numberOfChannel;
  unsigned char *data;

 public:
  unsigned int GetID();
  void Bind();

 public:
  Texture();
  Texture(const std::string &path, int textureLocation, int colorSpace,
          int colorCode);
  Texture(int width, int height);
  Texture(std::map<TextureTarget, std::string> textureFaces,
          int textureLocation, int colorSpace, int colorCode);
  ~Texture();
};
};  // namespace Core
