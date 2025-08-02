#pragma once
#include "glad/glad.h"
#include <map>
#include <string>
namespace Core {
struct CreateEmpty2DTextureDetail {
  int textureType;
  int colorSpace;
  int width;
  int height;
  int colorCode;
  int numberFormat;
};
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
  static int nextAvailableTextureId;
  static std::map<std::string, Core::Texture> loadedTextures;

 private:
  unsigned int id;
  int textureType;
  int textureLocation;

 public:
  int width, height, numberOfChannel;
  unsigned char *data;

 public:
  static Texture CreateEmpty2DTexture(const std::string &name,
                                      CreateEmpty2DTextureDetail textureDetail);

 private:
  void init(int location);
  void setTextureType(int textureType);
  void generateTexture(int textureType, int colorSpace, int width, int height,
                       int colorCode, int numberFormat);

 public:
  unsigned int GetID();
  int GetLocation();
  void SetParameterInt(int key, int value);
  void SetParameterFv(int key, float *fv);
  void Bind();
  void Unbind();

 public:
  Texture();
  Texture(const std::string &path, int textureLocation, int colorSpace,
          int colorCode);
  Texture(int width, int height, int textureLocation, int textureType,
          int colorSpace, int colorCode, int numberFormat, bool emptyParam);
  Texture(std::map<TextureTarget, std::string> textureFaces,
          int textureLocation, int colorSpace, int colorCode);
  ~Texture();
};
};  // namespace Core
