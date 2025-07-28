#include "particle/app.hpp"
#include <exception>
#include <memory>
#include <stdexcept>
#include "core/app.hpp"
#include "core/framebuffer/framebuffer.hpp"
#include "core/input/input.hpp"
#include "core/renderer/renderer.hpp"
#include "core/texture/texture.hpp"
#include "core/texture/texture_manager.hpp"
#include "core/time/time.hpp"
#include "core/window/window.hpp"
#include "core/world/world.hpp"
#include "editor/editor.hpp"
#include "particle/primitive/primitive.hpp"
#include "particle/simulation/simulation.hpp"
Particle::App::App() {
  core = Core::App();
  try {
    core.Init();
    Simulation::Init();
    Editor::Editor::Init();
    initFramebuffer();
    initRenderPlane();
  } catch (const std::exception& error) {
    throw error;
  }
}

void Particle::App::initFramebuffer() {
  framebuffer = std::make_shared<Core::Framebuffer>();
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(Core::TextureManager::CreateTexture(
          "main", Core::Window::GetWidth(), Core::Window::GetHeight())),
      GL_COLOR_ATTACHMENT0);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(Core::TextureManager::CreateTexture(
          "depth", Core::Window::GetWidth(), Core::Window::GetHeight())),
      GL_COLOR_ATTACHMENT1);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(Core::TextureManager::CreateTexture(
          "normal", Core::Window::GetWidth(), Core::Window::GetHeight())),
      GL_COLOR_ATTACHMENT2);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(Core::TextureManager::CreateTexture(
          "color", Core::Window::GetWidth(), Core::Window::GetHeight())),
      GL_COLOR_ATTACHMENT3);
  framebuffer->AttachRenderbuffer(std::make_shared<Core::Renderbuffer>(
      Core::Window::GetWidth(), Core::Window::GetHeight()));

  framebuffer->Bind();
  unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                 GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  glDrawBuffers(4, attachments);
  framebuffer->Unbind();
  if (!framebuffer->CheckStatus()) {
    throw std::runtime_error("framebuffer incomplete");
  }

  edgeDetectionFramebuffer = std::make_shared<Core::Framebuffer>();
  edgeDetectionFramebuffer->AttachTexture(
      std::make_shared<Core::Texture>(Core::TextureManager::CreateTexture(
          "outline", Core::Window::GetWidth(), Core::Window::GetHeight())),
      GL_COLOR_ATTACHMENT0);
  edgeDetectionFramebuffer->AttachRenderbuffer(
      std::make_shared<Core::Renderbuffer>(Core::Window::GetWidth(),
                                           Core::Window::GetHeight()));
  if (!edgeDetectionFramebuffer->CheckStatus()) {
    throw std::runtime_error("framebuffer incomplete");
  }
}

void Particle::App::initRenderPlane() {
  renderPlane = Primitive::CreatePlane("./shaders/screen/screen.vert",
                                       "./shaders/screen/screen.frag");
  renderPlane->name = "RenderPlane";

  edgeDetectionPlane = Primitive::CreatePlane(
      "./shaders/screen/screen.vert", "./shaders/screen/edge_detection.frag");
  edgeDetectionPlane->name = "Edge";
}

void Particle::App::Run() {
  try {
    while (!Core::Window::ShouldClose()) {
      Core::Time::Tick();
      Core::Window::PollEvent();
      Core::Input::PollMouse();
      Simulation::Update();
      Core::World::Tick();

      Editor::Editor::NewFrame();
      Editor::Editor::Update();

      framebuffer->Bind();
      Core::Renderer::AdjustViewport(false);
      Core::Renderer::DepthTest(true);
      Core::Renderer::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      Core::Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Core::Renderer::Render();

      framebuffer->Unbind();
      // EDGE DETECTION PASS
      edgeDetectionFramebuffer->Bind();
      Core::Renderer::AdjustViewport(false);
      Core::Renderer::DepthTest(false);
      Core::Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
      Core::Renderer::Clear(GL_COLOR_BUFFER_BIT);

      edgeDetectionPlane->mesh->material->Use();
      edgeDetectionPlane->mesh->material->SetInt(
          "colorTexture", Core::TextureManager::GetTextureLocation("color"));
      edgeDetectionPlane->mesh->material->SetInt(
          "normalTexture", Core::TextureManager::GetTextureLocation("normal"));
      edgeDetectionPlane->mesh->material->SetInt(
          "depthTexture", Core::TextureManager::GetTextureLocation("depth"));
      edgeDetectionPlane->mesh->BindVertexArray();
      framebuffer->BindTextures();
      glDrawElements(GL_TRIANGLES, edgeDetectionPlane->mesh->GetIndiceLength(),
                     GL_UNSIGNED_INT, 0);
      edgeDetectionFramebuffer->Unbind();

      Core::Renderer::AdjustViewport(true);
      Core::Renderer::DepthTest(false);
      Core::Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
      Core::Renderer::Clear(GL_COLOR_BUFFER_BIT);
      // TODO: find a way to just use the existing renderer here later
      renderPlane->mesh->material->Use();
      renderPlane->mesh->material->SetInt(
          "screenTexture",
          framebuffer->textureBuffers[0]->GetLocation() - GL_TEXTURE0);

      renderPlane->mesh->material->SetInt(
          "colorTexture", Core::TextureManager::GetTextureLocation("color"));
      renderPlane->mesh->material->SetInt(
          "normalTexture", Core::TextureManager::GetTextureLocation("normal"));
      renderPlane->mesh->material->SetInt(
          "depthTexture", Core::TextureManager::GetTextureLocation("depth"));

      renderPlane->mesh->material->SetInt(
          "outlineTexture",
          edgeDetectionFramebuffer->textureBuffers[0]->GetLocation() -
              GL_TEXTURE0);
      renderPlane->mesh->material->SetFloat("globalFloat",
                                            Simulation::globalFloat);
      renderPlane->mesh->material->SetFloat("globalFloat2",
                                            Simulation::globalFloat2);
      renderPlane->mesh->BindVertexArray();
      framebuffer->BindTextures();
      edgeDetectionFramebuffer->BindTextures();
      glDrawElements(GL_TRIANGLES, renderPlane->mesh->GetIndiceLength(),
                     GL_UNSIGNED_INT, 0);
      Editor::Editor::Render();

      Core::Window::SwapBuffer();
    }
  } catch (const std::exception& error) {
    throw error;
  }
}
