#include "core/renderer/renderer.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/material/material.hpp"
#include "core/object/object.hpp"
#include "core/procedural/procedural.hpp"
#include "core/shader/shader.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/static_light/static_light.hpp"
#include "core/texture/texture.hpp"
#include "core/window/window.hpp"

std::vector<std::shared_ptr<Core::Mesh>> Core::Renderer::renderQueue;
std::shared_ptr<Core::Object> Core::Renderer::skybox;

void Core::Renderer::Init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("failed to initiate GLAD");
  }

#if defined(WIN32)
  glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
#else
  glViewport(0, 0, Window::GetWidth() * 2, Window::GetHeight() * 2);
#endif

  skybox = std::make_shared<Core::Object>();
  skybox->name = "Skybox";
  Core::Shader shader{"./shaders/skybox/skybox.vert",
                      "./shaders/skybox/skybox.frag"};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Mesh> mesh = skybox->AddComponent<Core::Mesh>(material);
  skybox->mesh = mesh;
  Procedural::GenerateCube(mesh);

  std::map<Core::TextureTarget, std::string> textureFaces;
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_POSITIVE_X,
                       "./assets/skybox/right.jpg");
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_NEGATIVE_X,
                       "./assets/skybox/left.jpg");
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_POSITIVE_Y,
                       "./assets/skybox/top.jpg");
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_NEGATIVE_Y,
                       "./assets/skybox/bottom.jpg");
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_POSITIVE_Z,
                       "./assets/skybox/front.jpg");
  textureFaces.emplace(Core::TextureTarget::CUBE_MAP_NEGATIVE_Z,
                       "./assets/skybox/back.jpg");
  mesh->material->LoadTextureCubeMap("skyBox", textureFaces, GL_RGB, GL_RGB);
  mesh->material->SetInt("cubeTexture", Core::Texture::GetTextureID("skyBox"));
  DepthTest(true);
}

void Core::Renderer::DepthTest(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
    return;
  }
  glDisable(GL_DEPTH_TEST);
}

void Core::Renderer::AdjustViewport(int width, int height, bool scaleUp) {
  int multiplier = 1;
  if (scaleUp) {
    multiplier = 2;
  }
  glViewport(0, 0, width * multiplier, height * multiplier);
}

void Core::Renderer::Render() {
  for (std::shared_ptr<Mesh> &mesh : renderQueue) {
    if (!mesh->isActive) {
      continue;
    }
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }
    // FIXME: Do better than this
    if (owner->name == "RenderPlane" || owner->name == "Edge") {
      continue;
    }

    mesh->material->Use();
    mesh->material->SetMat4("model", owner->transform->GetTransformMatrix());
    mesh->material->SetMat4("view", StaticCamera::GetViewMatrix());
    mesh->material->SetMat4("projection", StaticCamera::GetProjectionMatrix());
    mesh->BindVertexArray();
    // TODO: add toggle for wireframe rendering somewhere
    /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LEQUAL);
    skybox->mesh->material->Use();
    skybox->mesh->material->SetMat4("view", StaticCamera::GetViewMatrix());
    skybox->mesh->material->SetMat4("projection",
                                    StaticCamera::GetProjectionMatrix());
    skybox->mesh->BindVertexArray();
    glDrawElements(GL_TRIANGLES, skybox->mesh->GetIndiceLength(),
                   GL_UNSIGNED_INT, 0);
    glDepthFunc(GL_LESS);
  }
}

void Core::Renderer::RenderShadowMap() {
  Shader shadowMappingShader = Shader{"./shaders/shadow/simple_shadow.vert",
                                      "./shaders/shadow/simple_shadow.frag"};
  glCullFace(GL_FRONT);
  for (std::shared_ptr<Mesh> &mesh : renderQueue) {
    if (!mesh->isActive) {
      continue;
    }
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }
    // FIXME: Do better than this
    if (owner->name == "RenderPlane" || owner->name == "Edge") {
      continue;
    }

    /*mesh->material->Use();*/
    shadowMappingShader.Use();
    shadowMappingShader.SetMat4("model",
                                owner->transform->GetTransformMatrix());
    shadowMappingShader.SetMat4("lightSpaceMatrix",
                                StaticLight::GetLightSpaceMatrix());
    mesh->BindVertexArray();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);
  }
  glCullFace(GL_BACK);
}

void Core::Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Core::Renderer::Clear(int bit) { glClear(bit); }

void Core::Renderer::AddToRenderQueue(std::shared_ptr<Mesh> mesh) {
  renderQueue.emplace_back(mesh);
}

void Core::Renderer::Cleanup() {}
