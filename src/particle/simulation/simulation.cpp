#include "particle/simulation/simulation.hpp"
#include <cstddef>
#include <format>
#include <memory>
#include <print>
#include <random>
#include <string>
#include <vector>
#include "GLFW/glfw3.h"
#include "core/components/point_light.hpp"
#include "core/components/rigidbody2d.hpp"
#include "core/components/spot_light.hpp"
#include "core/framebuffer/framebuffer.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/renderbuffer/renderbuffer.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/texture/texture.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "particle/primitive/primitive.hpp"

float Particle::Simulation::globalFloat = 0.0f;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::lightCubes;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::spotLightCubes;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::cubes;

glm::vec3 Particle::Simulation::lightColor = glm::vec3(1.0f);

const std::string vertexPath = "./shaders/diffuse/diffuse.vert";
const std::string diffuseFrag = "./shaders/diffuse/diffuse.frag";
const std::string demoFrag = "./shaders/diffuse/demo.frag";
const std::string diffuseFrag2 = "./shaders/diffuse/multi_light_diffuse.frag";
const std::string lightFrag = "./shaders/light/light.frag";
const std::string blinPhongFrag = "./shaders/diffuse/blin_phong.frag";
const std::string vertexPathV2 = "./shaders/diffuse/diffuse_v2.vert";

void Particle::Simulation::Init() {
  Core::StaticCamera::transform->position.z = 1.0f;

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

    light->transform->position = glm::vec3(dist(rng), dist(rng), -2.0f);

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

  std::shared_ptr<Core::Object> wall =
      Primitive::CreateUVSphere(vertexPathV2, blinPhongFrag, "Hello", 256, 256);
  wall->name = "Wall";
  wall->mesh->material->LoadTexture("./assets/brickwall.jpg", GL_SRGB, GL_RGB);
  wall->mesh->material->LoadTexture("./assets/brickwall_normal.jpg", GL_RGB,
                                    GL_RGB);
  wall->mesh->material->SetInt("material.diffuse", 1);
  wall->mesh->material->SetInt("material.specular", 1);
  wall->mesh->material->SetInt("material.normal", 2);
  cubes.emplace_back(wall);
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
  }
}

void Particle::Simulation::placeObjects() {
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

    light->transform->position = glm::vec3(dist(rng), dist(rng), -2.0f);

    lightCubes.emplace_back(light);
  }

  for (size_t i = 0; i < 1; i++) {
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

  for (size_t i = 0; i < 10; i++) {
    std::shared_ptr<Core::Object> obj =
        Primitive::CreateCube(vertexPath, diffuseFrag2);
    obj->name = std::format("Cube ({})", i);
    obj->mesh->material->LoadTexture("./assets/container2.png", GL_SRGB,
                                     GL_RGB);
    obj->mesh->material->LoadTexture("./assets/container2_specular.png", GL_RGB,
                                     GL_RGB);
    obj->mesh->material->LoadTexture("./assets/matrix.jpg", GL_SRGB, GL_RGB);
    obj->transform->position = glm::vec3(dist(rng), dist(rng), -2.0f);

    obj->mesh->material->SetVec3("objectColor", glm::vec3(0.3f, 0.8f, 0.2f));
    obj->mesh->material->SetInt("material.diffuse", 0);
    obj->mesh->material->SetInt("material.specular", 1);
    obj->mesh->material->SetInt("material.emission", 2);
    obj->mesh->material->SetFloat("material.shininess", 36.0f);

    obj->mesh->material->SetVec3("directionalLight.direction",
                                 glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)));
    obj->mesh->material->SetVec3("directionalLight.ambient", glm::vec3(0.5f));
    obj->mesh->material->SetVec3("directionalLight.diffuse", glm::vec3(1.0f));
    obj->mesh->material->SetVec3("directionalLight.specular", glm::vec3(1.0f));

    cubes.emplace_back(obj);
  }
}

void Particle::Simulation::objectUpdate() {
  for (std::shared_ptr<Core::Object> &cube : cubes) {
    for (size_t i = 0; i < lightCubes.size(); i++) {
      std::shared_ptr<Core::PointLight> pointLight =
          lightCubes[i]->GetComponent<Core::PointLight>();
      pointLight->SetMeshUniform(std::format("pointLight[{}]", i), cube);
    }

    for (size_t i = 0; i < spotLightCubes.size(); i++) {
      std::shared_ptr<Core::SpotLight> spotLight =
          spotLightCubes[i]->GetComponent<Core::SpotLight>();
      spotLight->SetMeshUniform(std::format("spotLight[{}]", i), cube);
    }

    cube->mesh->material->SetVec3(
        "cameraPosition", Core::StaticCamera::transform->GetWorldPosition());
    cube->mesh->material->SetFloat("globalFloat", globalFloat);
  }

  if (Core::Input::GetKey(GLFW_KEY_F)) {
    std::shared_ptr<Core::Object> obj =
        Primitive::CreatePlane(vertexPath, diffuseFrag);
    glm::vec3 wSpace = Core::Input::mouse.ToWorldPosition();
    obj->mesh->material->SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    obj->mesh->material->SetVec3("lightColor", glm::vec3(1.0f));

    obj->transform->scale = glm::vec3(0.05f);
    obj->transform->position = glm::vec3(wSpace.x, wSpace.y, 0.0f);
    obj->AddComponent<Core::Rigidbody2D>()->velocity = glm::vec2(5.1f, 2.0f);
  }
}
