#include "core/renderbuffer/renderbuffer.hpp"
#include <print>
#include "glad/glad.h"

Core::Renderbuffer::Renderbuffer() {}

Core::Renderbuffer::Renderbuffer(int width, int height) {
  glGenRenderbuffers(1, &id);
  Bind();
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  Unbind();
}

void Core::Renderbuffer::Bind() { glBindRenderbuffer(GL_RENDERBUFFER, id); }

void Core::Renderbuffer::Unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

unsigned int Core::Renderbuffer::GetID() { return id; }

Core::Renderbuffer::~Renderbuffer() { /*glDeleteRenderbuffers(1, &id);*/ }
