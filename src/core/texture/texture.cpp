#include "core/texture/texture.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include "stbi_image/stbi_image.h"

int Core::Texture::nextAvailableTextureId = 0;
std::map<std::string, Core::Texture> Core::Texture::loadedTextures;

Core::Texture Core::Texture::CreateEmptyCubeMapTexture(
    const std::string &name,
    struct CreateEmptyCubeMapTextureDetail textureDetail) {
  Texture texture;
  texture.init();
  texture.setTextureType(GL_TEXTURE_CUBE_MAP);
  texture.Bind();
  for (size_t i = 0; i < 6; i++) {
    texture.generateTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                            GL_DEPTH_COMPONENT, textureDetail.width,
                            textureDetail.height, GL_DEPTH_COMPONENT, GL_FLOAT);
  }
  texture.SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  texture.SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  texture.SetParameterInt(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  texture.SetParameterInt(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  texture.SetParameterInt(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  texture.Unbind();
  return texture;
}

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
  texture.Unbind();
  loadedTextures.emplace(name, texture);
  return texture;
}

Core::Texture Core::Texture::CreateCubeMapTextureFromImage(
    const std::string &name,
    CreateCubeMapTextureFromImageDetail textureDetail) {
  Texture texture;
  texture.init();
  texture.setTextureType(GL_TEXTURE_CUBE_MAP);
  texture.Bind();
  stbi_set_flip_vertically_on_load(false);
  for (auto &pair : textureDetail.textureFaces) {
    int width, height, numberOfChannel;
    unsigned char *data =
        stbi_load(pair.second.c_str(), &width, &height, &numberOfChannel, 0);
    if (data) {
      glTexImage2D(pair.first, 0, textureDetail.colorSpace, width, height, 0,
                   textureDetail.colorCode, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      stbi_image_free(data);
      throw std::runtime_error("failed to load texture");
    }
  }
  texture.SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture.SetParameterInt(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  texture.SetParameterInt(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  texture.SetParameterInt(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  texture.Unbind();
  loadedTextures.emplace(name, texture);
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
