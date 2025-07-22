#pragma once
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/texture/texture.hpp"
namespace Core {
class Framebuffer {
 private:
  unsigned int id;

 public:
  void AttachTexture(Core::Texture texture);
  void AttachRenderbuffer(Core::Renderbuffer renderbuffer);
  void Bind();
  void Unbind();
  bool CheckStatus();

 public:
  Framebuffer();
  ~Framebuffer();
};
};  // namespace Core
