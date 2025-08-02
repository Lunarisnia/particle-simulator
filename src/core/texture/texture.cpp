#include "core/texture/texture.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include "stbi_image/stbi_image.h"

int Core::Texture::nextAvailableTextureId = 0;
std::map<std::string, Core::Texture> Core::Texture::loadedTextures;

Core::Texture Core::Texture::CreateEmpty2DTexture(
    const std::string &name, struct CreateEmpty2DTextureDetail textureDetail) {
  Texture texture;
  texture.init();
  texture.setTextureType(textureDetail.textureType);
  texture.Bind();
  texture.generateTexture(textureDetail.textureType, textureDetail.colorSpace,
                          textureDetail.width, textureDetail.height,
                          textureDetail.colorCode, textureDetail.numberFormat);
  texture.SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture.SetParameterInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture.SetParameterInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture.Unbind();
  loadedTextures.emplace(name, texture);
  return texture;
}

Core::Texture Core::Texture::Create2DTextureFromImage(
    const std::string &name,
    struct Create2DTextureFromImageDetail textureDetail) {
  Texture texture;
  texture.init();
  texture.setTextureType(GL_TEXTURE_2D);
  texture.Bind();
  // Load image
  int width, height, numberOfChannel;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(textureDetail.path.c_str(), &width, &height,
                                  &numberOfChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, textureDetail.colorSpace, width, height, 0,
                 textureDetail.colorCode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("failed to load texture");
  }
  // Bind image
  texture.Unbind();
  loadedTextures.emplace(name, texture);
  return texture;
}

Core::Texture Core::Texture::CreateCubeMapTextureFromImage(
    const std::string &name,
    CreateCubeMapTextureFromImageDetail textureDetail) {
  Texture texture;
  texture.init();
  texture.Bind();

  texture.Unbind();
  return texture;
}

int Core::Texture::GetTextureID(const std::string &name) {
  if (loadedTextures.count(name) < 1) {
    return 0;
  }
  return loadedTextures.at(name).GetLocation() - GL_TEXTURE0;
}

void Core::Texture::init() {
  glGenTextures(1, &id);
  textureLocation = GL_TEXTURE0 + nextAvailableTextureId;
  nextAvailableTextureId++;
}

void Core::Texture::generateTexture(int textureType, int colorSpace, int width,
                                    int height, int colorCode,
                                    int numberFormat) {
  glTexImage2D(textureType, 0, colorSpace, width, height, 0, colorCode,
               numberFormat, nullptr);
}

void Core::Texture::setTextureType(int type) { textureType = type; }

Core::Texture::Texture() {}

Core::Texture::Texture(int width, int height, int textureLocation,
                       int textureType, int colorSpace, int colorCode,
                       int numberFormat, bool emptyParam)
    : textureLocation(textureLocation), textureType(textureType) {
  glGenTextures(1, &id);
  glBindTexture(textureType, id);

  if (emptyParam) {
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  glTexImage2D(textureType, 0, colorSpace, width, height, 0, colorCode,
               numberFormat, nullptr);
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

void Core::Texture::SetParameterInt(int key, int value) {
  Bind();
  glTexParameteri(textureType, key, value);
}
void Core::Texture::SetParameterFv(int key, float *fv) {
  Bind();
  glTexParameterfv(textureType, key, fv);
}

void Core::Texture::Bind() {
  glActiveTexture(textureLocation);
  glBindTexture(textureType, id);
}

void Core::Texture::Unbind() { glBindTexture(textureType, 0); }

unsigned int Core::Texture::GetID() { return id; }
int Core::Texture::GetLocation() { return textureLocation; }

Core::Texture::~Texture() {}
