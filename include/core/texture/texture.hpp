#pragma once
#include <string>
namespace Core {
class Texture {
 private:
  unsigned int id;

 public:
  int width, height, numberOfChannel;
  unsigned char *data;

 public:
  void Bind();

 public:
  Texture();
  Texture(const std::string &path);
  ~Texture();
};
};  // namespace Core
