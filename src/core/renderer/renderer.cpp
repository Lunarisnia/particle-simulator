#include "core/renderer/renderer.hpp"
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/components/point_light.hpp"
#include "core/material/material.hpp"
#include "core/object/object.hpp"
#include "core/primitive/primitive.hpp"
#include "core/procedural/procedural.hpp"
#include "core/shader/shader.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/static_light/static_light.hpp"
#include "core/texture/texture.hpp"
#include "core/time/time.hpp"
#include "core/window/window.hpp"
#include "glm/ext/matrix_float4x4.hpp"

std::vector<std::shared_ptr<Core::Mesh>> Core::Renderer::renderQueue;
std::shared_ptr<Core::Object> Core::Renderer::skybox;
Core::Shader Core::Renderer::shadowMappingShader;
Core::Shader Core::Renderer::visualizeNormalShader;
bool Core::Renderer::enableSkybox = true;
bool Core::Renderer::drawNormal = false;
std::shared_ptr<Core::Object> Core::Renderer::viewport;

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
  shadowMappingShader = Shader::CreateShaderWithGeometry(
      "./shaders/shadow/shadow_cube_map.vert",
      "./shaders/shadow/shadow_cube_map_geometry.glsl",
      "./shaders/shadow/shadow_cube_map.frag");
  DepthTest(true);

  Core::Shader viewportShader{"./shaders/screen/screen.vert",
                              "./shaders/screen/screen.frag"};
  viewport = Core::Primitive::CreatePlane(viewportShader);
  viewport->name = "RenderPlane";

  visualizeNormalShader = Shader::CreateShaderWithGeometry(
      "./shaders/normal-viz/normal.vert", "./shaders/normal-viz/normal.glsl",
      "./shaders/normal-viz/normal.frag");
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
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }
    if (!mesh->isActive || !owner->isActive) {
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
    mesh->material->SetFloat("currentTime", Time::timeSinceStartup);
    mesh->BindVertexArray();

    glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);

    if (drawNormal) {
      visualizeNormalShader.Use();
      visualizeNormalShader.SetMat4("model",
                                    owner->transform->GetTransformMatrix());
      visualizeNormalShader.SetMat4("view", StaticCamera::GetViewMatrix());
      visualizeNormalShader.SetMat4("projection",
                                    StaticCamera::GetProjectionMatrix());
      visualizeNormalShader.SetFloat("currentTime", Time::timeSinceStartup);
      mesh->BindVertexArray();
      glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);
    }

    if (enableSkybox) {
      glDepthFunc(GL_LEQUAL);
      skybox->mesh->material->Use();
      skybox->mesh->material->SetMat4("view", StaticCamera::GetViewMatrix());
      skybox->mesh->material->SetMat4("projection",
                                      StaticCamera::GetProjectionMatrix());
      skybox->mesh->BindVertexArray();
      glDrawElements(GL_TRIANGLES, skybox->mesh->GetIndiceLength(),
                     GL_UNSIGNED_INT, 0);
    }
    glDepthFunc(GL_LESS);
  }
}

void Core::Renderer::RenderViewport() {
  viewport->mesh->material->Use();
  viewport->mesh->material->SetInt("screenTexture",
                                   Core::Texture::GetTextureID("main"));
  viewport->mesh->material->SetInt("colorTexture",
                                   Core::Texture::GetTextureID("color"));
  viewport->mesh->material->SetInt("normalTexture",
                                   Core::Texture::GetTextureID("normal"));
  viewport->mesh->material->SetInt("depthTexture",
                                   Core::Texture::GetTextureID("depth"));
  viewport->mesh->material->SetInt(
      "shadowTexture", Core::Texture::GetTextureID("shadowCubeMap"));

  viewport->mesh->material->SetInt("outlineTexture",
                                   Core::Texture::GetTextureID("outline"));
  glDrawElements(GL_TRIANGLES, viewport->mesh->GetIndiceLength(),
                 GL_UNSIGNED_INT, 0);
}

void Core::Renderer::RenderShadowCubeMap() {
  glCullFace(GL_FRONT);
  for (std::shared_ptr<Mesh> &mesh : renderQueue) {
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }
    if (!mesh->isActive || !owner->isActive) {
      continue;
    }
    // FIXME: Do better than this
    if (owner->name == "RenderPlane" || owner->name == "Edge") {
      continue;
    }

    // TODO: Try continuing after the hooking of the cubemap shader part
    /*mesh->material->Use();*/
    shadowMappingShader.Use();
    shadowMappingShader.SetMat4("model",
                                owner->transform->GetTransformMatrix());
    if (owner->GetComponent<Core::PointLight>() != nullptr) {
      std::shared_ptr<Core::PointLight> pointLight =
          owner->GetComponent<Core::PointLight>();
      std::vector<glm::mat4> shadowMapMatrices =
          pointLight->GetCubeMapLightMatrix();
      shadowMappingShader.SetFloat("farPlane", 25.0f);
      shadowMappingShader.SetVec3("lightPos", owner->transform->position);
      for (size_t i = 0; i < 6; i++) {
        shadowMappingShader.SetMat4(std::format("shadowMatrices[{}]", i),
                                    shadowMapMatrices[i]);
      }
    }
    mesh->BindVertexArray();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);
  }
  glCullFace(GL_BACK);
}

void Core::Renderer::RenderShadowMap() {
  Shader shadowMappingShader = Shader{"./shaders/shadow/simple_shadow.vert",
                                      "./shaders/shadow/simple_shadow.frag"};
  glCullFace(GL_FRONT);
  for (std::shared_ptr<Mesh> &mesh : renderQueue) {
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }
    if (!mesh->isActive || !owner->isActive) {
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
