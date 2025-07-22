#pragma once
namespace Core {
class Renderbuffer {
 private:
  unsigned int id;

 public:
  void Bind();
  void Unbind();
  unsigned int GetID();

 public:
  Renderbuffer();
  Renderbuffer(int width, int height);
  ~Renderbuffer();
};
};  // namespace Core
