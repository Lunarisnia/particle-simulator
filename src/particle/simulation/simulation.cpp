#include "particle/simulation/simulation.hpp"
#include <cstddef>
#include <exception>
#include <format>
#include <memory>
#include <print>
#include <random>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/point_light.hpp"
#include "core/components/spot_light.hpp"
#include "core/object/object.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/static_light/static_light.hpp"
#include "core/texture/texture.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include "particle/model/model.hpp"
#include "particle/primitive/primitive.hpp"

float Particle::Simulation::globalFloat = 0.0f;
float Particle::Simulation::globalFloat2 = 0.0f;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::lightCubes;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::spotLightCubes;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::cubes;

glm::vec3 Particle::Simulation::lightColor = glm::vec3(1.0f);

const std::string vertexPath = "./shaders/diffuse/diffuse.vert";
const std::string diffuseFrag = "./shaders/diffuse/diffuse.frag";
const std::string cellShaderFrag = "./shaders/diffuse/cell_shader.frag";
const std::string diffuseFrag2 = "./shaders/diffuse/multi_light_diffuse.frag";
const std::string lightFrag = "./shaders/light/light.frag";
const std::string blinPhongFrag = "./shaders/diffuse/blin_phong.frag";
const std::string vertexPathV2 = "./shaders/diffuse/diffuse_v2.vert";

void Particle::Simulation::Init() {
  Core::StaticCamera::transform->position.z = 4.0f;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<float> dist(-3.5f, 3.5f);
  for (size_t i = 0; i < 2; i++) {
    std::shared_ptr<Core::Object> light =
        Primitive::CreateCube(vertexPath, lightFrag);
    std::shared_ptr<Core::PointLight> pointLight =
        light->AddComponent<Core::PointLight>();
    pointLight->ambient = glm::vec3(0.5f);
    pointLight->diffuse = glm::vec3(1.0f);
    pointLight->specular = glm::vec3(1.0f);

    pointLight->constant = 1.0f;
    pointLight->linear = 0.14f;
    pointLight->quadratic = 0.07f;

    light->name = std::format("Point Light ({})", i);
    light->mesh->material->SetVec3("lightColor", glm::vec3(1.0f));

    /*light->transform->position = glm::vec3(dist(rng), dist(rng), -2.0f);*/
    light->transform->position = glm::vec3(3.0f, 1.0f, 4.0f);

    lightCubes.emplace_back(light);
  }
  for (size_t i = 0; i < 0; i++) {
    std::shared_ptr<Core::Object> light =
        Primitive::CreateCube(vertexPath, lightFrag);
    std::shared_ptr<Core::SpotLight> spotLight =
        light->AddComponent<Core::SpotLight>();
    spotLight->direction = glm::vec3(0.0f, 0.0f, -1.0f);
    spotLight->ambient = glm::vec3(0.5f);
    spotLight->diffuse = glm::vec3(1.0f);
    spotLight->specular = glm::vec3(1.0f);

    spotLight->cutoff = glm::radians(45.0f);
    spotLight->outerCutoff = glm::radians(50.0f);
    spotLight->constant = 1.0f;
    spotLight->linear = 0.14f;
    spotLight->quadratic = 0.0007f;

    light->name = std::format("Spot Light ({})", i);
    light->mesh->material->SetVec3("lightColor", glm::vec3(1.0f));

    light->transform->position = glm::vec3(0.0f, 0.0f, 4.0f);

    spotLightCubes.emplace_back(light);
  }

  const std::string brickWallTexture = "./assets/brickwall.jpg";
  const std::string brickWallNormal = "./assets/brickwall_normal.jpg";
  std::shared_ptr<Core::Object> ball =
      Primitive::CreateUVSphere(vertexPathV2, blinPhongFrag, "Hello", 256, 256);
  ball->transform->position = glm::vec3(-2.0f, 0.0f, 0.0f);
  ball->name = "Ball";
  ball->mesh->material->LoadTexture(brickWallTexture, GL_SRGB, GL_RGB);
  ball->mesh->material->LoadTexture(brickWallNormal, GL_RGB, GL_RGB);
  ball->mesh->material->SetInt("material.diffuse",
                               Core::Texture::GetTextureID(brickWallTexture));
  ball->mesh->material->SetInt("material.specular",
                               Core::Texture::GetTextureID(brickWallTexture));
  ball->mesh->material->SetInt("material.normal",
                               Core::Texture::GetTextureID(brickWallNormal));
  cubes.emplace_back(ball);

  const std::string containerTexture = "./assets/container2.png";
  const std::string containerSpecular = "./assets/container2_specular.png";
  std::shared_ptr<Core::Object> cube =
      Primitive::CreateCube(vertexPathV2, blinPhongFrag);
  cube->transform->position = glm::vec3(0.0f, -2.0f, -1.0f);
  cube->transform->scale = glm::vec3(10.0f, 1.0f, 10.0f);
  cube->name = "Floor";
  cube->mesh->material->LoadTexture(containerTexture, GL_SRGB_ALPHA, GL_RGBA);
  cube->mesh->material->LoadTexture(containerSpecular, GL_SRGB_ALPHA, GL_RGBA);
  cube->mesh->material->SetInt("material.diffuse",
                               Core::Texture::GetTextureID(containerTexture));
  cube->mesh->material->SetInt("material.specular",
                               Core::Texture::GetTextureID(containerSpecular));
  cube->mesh->material->SetInt("material.normal",
                               Core::Texture::GetTextureID(brickWallNormal));
  cubes.emplace_back(cube);

  try {
    Particle::Model model;
    /*model.LoadModel("./assets/honkai-star-rail-jingliu/source/JingiluV3.fbx");*/
    /*model.LoadModel("./assets/bed-room/bed-room.obj");*/
    /*model.StoreMeshes(cubes);*/
  } catch (std::exception &error) {
    std::println("{}", error.what());
  }
}

// TODO: Update TBN matrix every frame
void Particle::Simulation::Update() {
  for (std::shared_ptr<Core::Object> &object : cubes) {
    for (size_t i = 0; i < lightCubes.size(); i++) {
      std::shared_ptr<Core::PointLight> pointLight =
          lightCubes[i]->GetComponent<Core::PointLight>();
      pointLight->SetMeshUniform(std::format("pointLight[{}]", i), object);
    }

    for (size_t i = 0; i < spotLightCubes.size(); i++) {
      std::shared_ptr<Core::SpotLight> spotLight =
          spotLightCubes[i]->GetComponent<Core::SpotLight>();
      spotLight->SetMeshUniform(std::format("spotLight[{}]", i), object);
    }

    object->mesh->material->SetVec3(
        "camera.position", Core::StaticCamera::transform->GetWorldPosition());
    object->mesh->material->SetFloat("globalFloat", globalFloat);
    object->mesh->material->SetFloat("globalFloat2", globalFloat2);
    object->mesh->material->SetMat4("lightSpaceMatrix",
                                    Core::StaticLight::GetLightSpaceMatrix());
    object->mesh->material->SetInt("shadowMap",
                                   Core::Texture::GetTextureID("shadowMap"));
  }
}
