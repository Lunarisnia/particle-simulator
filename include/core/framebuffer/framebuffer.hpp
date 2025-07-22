#pragma once
#include <memory>
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/texture/texture.hpp"
namespace Core {
class Framebuffer {
 private:
  unsigned int id;

 public:
  std::shared_ptr<Core::Texture> textureBuffer;
  std::shared_ptr<Core::Renderbuffer> renderbuffer;

 public:
  void AttachTexture(std::shared_ptr<Core::Texture> texture);
  void AttachRenderbuffer(std::shared_ptr<Core::Renderbuffer> renderbuffer);
  void Bind();
  void Unbind();
  bool CheckStatus();

 public:
  Framebuffer();
  ~Framebuffer();
};
};  // namespace Core
