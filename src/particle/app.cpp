#include "particle/app.hpp"
#include <cups/raster.h>
#include <exception>
#include <memory>
#include <stdexcept>
#include "core/app.hpp"
#include "core/framebuffer/framebuffer.hpp"
#include "core/input/input.hpp"
#include "core/renderer/renderer.hpp"
#include "core/texture/texture.hpp"
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
  Core::CreateEmpty2DTextureDetail detail;
  detail.width = Core::Window::GetWidth();
  detail.height = Core::Window::GetHeight();
  detail.textureType = GL_TEXTURE_2D;
  detail.colorSpace = GL_RGB;
  detail.colorCode = GL_RGB;
  detail.numberFormat = GL_UNSIGNED_BYTE;
  framebuffer = std::make_shared<Core::Framebuffer>();
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("main", detail)),
      GL_COLOR_ATTACHMENT0);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("depth", detail)),
      GL_COLOR_ATTACHMENT1);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("normal", detail)),
      GL_COLOR_ATTACHMENT2);
  framebuffer->AttachTexture(
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("color", detail)),
      GL_COLOR_ATTACHMENT3);
  framebuffer->AttachRenderbuffer(
      std::make_shared<Core::Renderbuffer>(detail.width, detail.height));

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
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("outline", detail)),
      GL_COLOR_ATTACHMENT0);
  edgeDetectionFramebuffer->AttachRenderbuffer(
      std::make_shared<Core::Renderbuffer>(detail.width, detail.height));
  if (!edgeDetectionFramebuffer->CheckStatus()) {
    throw std::runtime_error("framebuffer incomplete");
  }

  // SHADOW MAP
  Core::CreateEmpty2DTextureDetail shadowDetail;
  shadowDetail.width = 1024;
  shadowDetail.height = 1024;
  shadowDetail.textureType = GL_TEXTURE_2D;
  shadowDetail.colorSpace = GL_DEPTH_COMPONENT;
  shadowDetail.colorCode = GL_DEPTH_COMPONENT;
  shadowDetail.numberFormat = GL_FLOAT;
  std::shared_ptr<Core::Texture> shadowTexture =
      std::make_shared<Core::Texture>(
          Core::Texture::CreateEmpty2DTexture("shadowMap", shadowDetail));
  shadowTexture->SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  shadowTexture->SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  shadowTexture->SetParameterInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
  shadowTexture->SetParameterInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
  shadowMapFramebuffer = std::make_shared<Core::Framebuffer>();
  shadowMapFramebuffer->AttachTexture(shadowTexture, GL_DEPTH_ATTACHMENT);
  shadowMapFramebuffer->Bind();
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (!shadowMapFramebuffer->CheckStatus()) {
    throw std::runtime_error("fb incomplete");
  }
  shadowMapFramebuffer->Unbind();

  // Cubemap ShadowMap
  Core::CreateEmptyCubeMapTextureDetail cubeShadowDetail;
  cubeShadowDetail.width = 1024;
  cubeShadowDetail.height = 1024;
  std::shared_ptr<Core::Texture> cubeMapShadowTexture =
      std::make_shared<Core::Texture>(Core::Texture::CreateEmptyCubeMapTexture(
          "cubeMapShadowMap", cubeShadowDetail));
  cubeMapShadowMapFramebuffer = std::make_shared<Core::Framebuffer>();
  cubeMapShadowMapFramebuffer->AttachTexture(cubeMapShadowTexture,
                                             GL_DEPTH_ATTACHMENT, true);
  cubeMapShadowMapFramebuffer->Bind();
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (!cubeMapShadowMapFramebuffer->CheckStatus()) {
    throw std::runtime_error("fb incomplete");
  }
  cubeMapShadowMapFramebuffer->Unbind();
}

void Particle::App::initRenderPlane() {
  renderPlane = Primitive::CreatePlane("./shaders/screen/screen.vert",
                                       "./shaders/screen/screen.frag");
  renderPlane->name = "RenderPlane";

  edgeDetectionPlane = Primitive::CreatePlane(
      "./shaders/screen/screen.vert", "./shaders/screen/edge_detection.frag");
  edgeDetectionPlane->name = "Edge";
}

void Particle::App::edgeDetectionPass() {
  // EDGE DETECTION PASS
  edgeDetectionFramebuffer->Bind();
  Core::Renderer::AdjustViewport(Core::Window::GetWidth(),
                                 Core::Window::GetHeight(), false);
  Core::Renderer::DepthTest(false);
  Core::Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  Core::Renderer::Clear(GL_COLOR_BUFFER_BIT);

  edgeDetectionPlane->mesh->material->Use();
  edgeDetectionPlane->mesh->material->SetInt(
      "colorTexture", Core::Texture::GetTextureID("color"));
  edgeDetectionPlane->mesh->material->SetInt(
      "normalTexture", Core::Texture::GetTextureID("normal"));
  edgeDetectionPlane->mesh->material->SetInt(
      "depthTexture", Core::Texture::GetTextureID("depth"));
  edgeDetectionPlane->mesh->BindVertexArray();
  framebuffer->BindTextures();
  glDrawElements(GL_TRIANGLES, edgeDetectionPlane->mesh->GetIndiceLength(),
                 GL_UNSIGNED_INT, 0);
  edgeDetectionFramebuffer->Unbind();

  Core::Renderer::AdjustViewport(Core::Window::GetWidth(),
                                 Core::Window::GetHeight(), true);
  Core::Renderer::DepthTest(false);
  Core::Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  Core::Renderer::Clear(GL_COLOR_BUFFER_BIT);
  // TODO: find a way to just use the existing renderer here later
  renderPlane->mesh->material->Use();
  renderPlane->mesh->material->SetInt(
      "screenTexture",
      framebuffer->textureBuffers[0]->GetLocation() - GL_TEXTURE0);

  renderPlane->mesh->material->SetInt("colorTexture",
                                      Core::Texture::GetTextureID("color"));
  renderPlane->mesh->material->SetInt("normalTexture",
                                      Core::Texture::GetTextureID("normal"));
  renderPlane->mesh->material->SetInt("depthTexture",
                                      Core::Texture::GetTextureID("depth"));

  renderPlane->mesh->material->SetInt(
      "outlineTexture",
      edgeDetectionFramebuffer->textureBuffers[0]->GetLocation() - GL_TEXTURE0);
  renderPlane->mesh->material->SetFloat("globalFloat", Simulation::globalFloat);
  renderPlane->mesh->material->SetFloat("globalFloat2",
                                        Simulation::globalFloat2);
  renderPlane->mesh->BindVertexArray();
  framebuffer->BindTextures();
  edgeDetectionFramebuffer->BindTextures();
  glDrawElements(GL_TRIANGLES, renderPlane->mesh->GetIndiceLength(),
                 GL_UNSIGNED_INT, 0);
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
      cubeMapShadowMapFramebuffer->Bind();
      Core::Renderer::AdjustViewport(1024, 1024, false);
      Core::Renderer::DepthTest(true);
      Core::Renderer::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      Core::Renderer::Clear(GL_DEPTH_BUFFER_BIT);

      Core::Renderer::RenderShadowCubeMap();

      cubeMapShadowMapFramebuffer->Unbind();

      shadowMapFramebuffer->Bind();
      Core::Renderer::AdjustViewport(1024, 1024, false);
      Core::Renderer::DepthTest(true);
      Core::Renderer::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      Core::Renderer::Clear(GL_DEPTH_BUFFER_BIT);

      Core::Renderer::RenderShadowMap();

      shadowMapFramebuffer->Unbind();

      framebuffer->Bind();
      Core::Renderer::AdjustViewport(Core::Window::GetWidth(),
                                     Core::Window::GetHeight(), false);
      Core::Renderer::DepthTest(true);
      Core::Renderer::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      Core::Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Core::Renderer::Render();

      framebuffer->Unbind();

      Core::Renderer::AdjustViewport(Core::Window::GetWidth(),
                                     Core::Window::GetHeight(), true);
      Core::Renderer::DepthTest(false);
      Core::Renderer::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
      Core::Renderer::Clear(GL_COLOR_BUFFER_BIT);
      // TODO: find a way to just use the existing renderer here later
      renderPlane->mesh->material->Use();
      renderPlane->mesh->material->SetInt("screenTexture",
                                          Core::Texture::GetTextureID("main"));
      renderPlane->mesh->material->SetInt("colorTexture",
                                          Core::Texture::GetTextureID("color"));
      renderPlane->mesh->material->SetInt(
          "normalTexture", Core::Texture::GetTextureID("normal"));
      renderPlane->mesh->material->SetInt("depthTexture",
                                          Core::Texture::GetTextureID("depth"));
      renderPlane->mesh->material->SetInt(
          "shadowTexture", Core::Texture::GetTextureID("shadowMap"));

      renderPlane->mesh->material->SetInt(
          "outlineTexture", Core::Texture::GetTextureID("outline"));
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
