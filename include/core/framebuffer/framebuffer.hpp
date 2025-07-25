#pragma once
#include <memory>
#include <vector>
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/texture/texture.hpp"
namespace Core {
class Framebuffer {
 private:
  unsigned int id;

 public:
  std::vector<std::shared_ptr<Core::Texture>> textureBuffers;
  std::vector<std::shared_ptr<Core::Renderbuffer>> renderbuffers;

 public:
  void AttachTexture(std::shared_ptr<Core::Texture> texture,
                     unsigned int attachment);
  void AttachRenderbuffer(std::shared_ptr<Core::Renderbuffer> renderbuffer);
  void Bind();
  void BindTextures();
  void Unbind();
  bool CheckStatus();

 public:
  Framebuffer();
  ~Framebuffer();
};
};  // namespace Core
