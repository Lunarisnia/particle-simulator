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

struct CreateEmpty2DTextureDetail {
  int textureType;
  int colorSpace;
  int width;
  int height;
  int colorCode;
  int numberFormat;
};

struct Create2DTextureFromImageDetail {
  std::string path;
  int colorSpace;
  int colorCode;
};

struct CreateCubeMapTextureFromImageDetail {
  std::map<TextureTarget, std::string> textureFaces;
  int colorSpace;
  int colorCode;
};

struct CreateEmptyCubeMapTextureDetail {
  int width;
  int height;
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
  static Texture Create2DTextureFromImage(
      const std::string &name, Create2DTextureFromImageDetail textureDetail);
  static Texture CreateCubeMapTextureFromImage(
      const std::string &name,
      CreateCubeMapTextureFromImageDetail textureDetail);
  static Texture CreateEmptyCubeMapTexture(
      const std::string &name, CreateEmptyCubeMapTextureDetail textureDetail);
  static int GetTextureID(const std::string &name);

 private:
  void init();
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
  ~Texture();
};
};  // namespace Core
