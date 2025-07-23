#pragma once
#include <string>
namespace Core {
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
  ~Texture();
};
};  // namespace Core
