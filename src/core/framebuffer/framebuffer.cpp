#include "core/framebuffer/framebuffer.hpp"
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/texture/texture.hpp"
#include "glad/glad.h"

Core::Framebuffer::Framebuffer() { glGenFramebuffers(1, &id); }

void Core::Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }
void Core::Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Core::Framebuffer::AttachTexture(Texture texture) {
  Bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture.GetID(), 0);
  Unbind();
}

void Core::Framebuffer::AttachRenderbuffer(Core::Renderbuffer renderbuffer) {
  Bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, renderbuffer.GetID());
  Unbind();
}

bool Core::Framebuffer::CheckStatus() {
  Bind();
  bool status =
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  Unbind();
  return status;
}

Core::Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &id); }
