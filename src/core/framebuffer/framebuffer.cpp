#include "core/framebuffer/framebuffer.hpp"
#include <memory>
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/texture/texture.hpp"
#include "glad/glad.h"

Core::Framebuffer::Framebuffer() { glGenFramebuffers(1, &id); }

void Core::Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }
void Core::Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Core::Framebuffer::AttachTexture(std::shared_ptr<Texture> texture,
                                      unsigned int attachment) {
  Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
                         texture->GetID(), 0);
  textureBuffers.emplace_back(texture);
  Unbind();
}

void Core::Framebuffer::BindTextures() {
  for (std::shared_ptr<Core::Texture>& texture : textureBuffers) {
    texture->Bind();
  }
}

void Core::Framebuffer::AttachRenderbuffer(
    std::shared_ptr<Core::Renderbuffer> renderbuffer) {
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, renderbuffer->GetID());
  renderbuffers.emplace_back(renderbuffer);
  Unbind();
}

bool Core::Framebuffer::CheckStatus() {
  Bind();
  bool status =
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  Unbind();
  return status;
}

Core::Framebuffer::~Framebuffer() { /*glDeleteFramebuffers(1, &id);*/ }
