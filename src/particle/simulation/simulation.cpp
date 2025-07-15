#include "particle/simulation/simulation.hpp"
#include <cstddef>
#include <format>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include "GLFW/glfw3.h"
#include "core/components/rigidbody2d.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/static_camera/static_camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/trigonometric.hpp"
#include "particle/primitive/primitive.hpp"

float Particle::Simulation::globalFloat = 0.0f;
std::shared_ptr<Core::Object> Particle::Simulation::cube;
std::shared_ptr<Core::Object> Particle::Simulation::lightCube;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::cubes;
std::vector<std::shared_ptr<Core::Object>> Particle::Simulation::lightCubes;

glm::vec3 Particle::Simulation::lightColor = glm::vec3(1.0f);

const std::string vertexPath = "./shaders/diffuse/diffuse.vert";
const std::string diffuseFrag = "./shaders/diffuse/diffuse.frag";
const std::string diffuseFrag2 = "./shaders/diffuse/multi_light_diffuse.frag";
const std::string lightFrag = "./shaders/light/light.frag";

// TODO: Study lighting maps
void Particle::Simulation::Init() {
  Core::StaticCamera::transform->position.z = 1.0f;

  lightCube = Primitive::CreateCube(vertexPath, lightFrag);
  lightCube->transform->position = glm::vec3(1.5f, 1.5f, -1.5f);
  lightCube->name = "PointLight1";

  /*cube = Primitive::CreateCube(vertexPath, diffuseFrag2);*/
  /*cube->name = "BetaCube";*/

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<float> dist(
      -3.5f, 3.5f);  // distribution in range [1, 6]

  for (size_t i = 0; i < 10; i++) {
    std::shared_ptr<Core::Object> obj =
        Primitive::CreateCube(vertexPath, diffuseFrag2);
    obj->name = std::format("Cube ({})", i);
    obj->mesh->material->LoadTexture("./assets/container2.png", GL_TEXTURE0,
                                     GL_RGBA);
    obj->mesh->material->LoadTexture("./assets/container2_specular.png",
                                     GL_TEXTURE1, GL_RGBA);
    obj->mesh->material->LoadTexture("./assets/matrix.jpg", GL_TEXTURE2,
                                     GL_RGB);
    /*obj->transform->position.z = -1.0f;*/
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

    obj->mesh->material->SetVec3("pointLight.ambient", glm::vec3(0.5f));
    obj->mesh->material->SetVec3("pointLight.diffuse", glm::vec3(1.0f));
    obj->mesh->material->SetVec3("pointLight.specular", glm::vec3(1.0f));

    /*obj->mesh->material->SetVec3("light.ambient", glm::vec3(1.0f));*/
    /*obj->mesh->material->SetVec3("light.specular", glm::vec3(1.0f));*/
    /*obj->mesh->material->SetFloat("light.constant", 1.0f);*/
    /*obj->mesh->material->SetFloat("light.linear", 0.14f);*/
    /*obj->mesh->material->SetFloat("light.quadratic", 0.07f);*/
    /*obj->mesh->material->SetVec3("light.spotDirection",*/
    /*                             glm::vec3(0.0f, -1.0f, 0.0f));*/
    /*obj->mesh->material->SetFloat("light.cutoff", glm::radians(45.0f));*/
    /*obj->mesh->material->SetFloat("light.outerCutoff", glm::radians(50.0f));*/

    cubes.emplace_back(obj);
  }
}

void Particle::Simulation::Update() {
  glm::vec3 lightPosWorldSpace =
      lightCube->transform->GetTransformMatrix() *
      glm::vec4(lightCube->transform->position, 1.0f);
  glm::vec3 cameraWorldSpace =
      Core::StaticCamera::transform->GetTransformMatrix() *
      glm::vec4(Core::StaticCamera::transform->position, 1.0f);

  // TODO: add stbi_image and texture support
  lightCube->mesh->material->SetVec3("lightColor", lightColor);
  for (std::shared_ptr<Core::Object> &cube : cubes) {
    cube->mesh->material->SetVec3("pointLight.position", lightPosWorldSpace);
    cube->mesh->material->SetVec3("light.position", lightPosWorldSpace);
    cube->mesh->material->SetVec3("cameraPosition", cameraWorldSpace);
    cube->mesh->material->SetFloat("globalFloat", globalFloat);
    cube->mesh->material->SetVec3("light.diffuse", lightColor);
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
