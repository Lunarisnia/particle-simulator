#include "particle/simulation/simulation.hpp"
#include <memory>
#include <string>
#include "GLFW/glfw3.h"
#include "core/components/rigidbody2d.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/static_camera/static_camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "particle/primitive/primitive.hpp"

float Particle::Simulation::globalFloat = 0.0f;
std::shared_ptr<Core::Object> Particle::Simulation::cube;
std::shared_ptr<Core::Object> Particle::Simulation::lightCube;
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
  lightCube->name = "Test2";

  cube = Primitive::CreateCube(vertexPath, diffuseFrag2);
  cube->name = "Test1";
  cube->mesh->material->LoadTexture("./assets/container2.png", GL_TEXTURE0,
                                    GL_RGBA);
  cube->mesh->material->LoadTexture("./assets/container2_specular.png",
                                    GL_TEXTURE1, GL_RGBA);
  cube->mesh->material->LoadTexture("./assets/matrix.jpg", GL_TEXTURE2, GL_RGB);
  cube->transform->position.z = -1.0f;
  cube->mesh->material->SetVec3("objectColor", glm::vec3(0.3f, 0.8f, 0.2f));
  cube->mesh->material->SetInt("material.diffuse", 0);
  cube->mesh->material->SetInt("material.specular", 1);
  cube->mesh->material->SetInt("material.emission", 2);
  cube->mesh->material->SetFloat("material.shininess", 36.0f);

  cube->mesh->material->SetVec3("directionalLight.direction",
                                glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f)));
  cube->mesh->material->SetVec3("directionalLight.ambient", glm::vec3(0.5f));
  cube->mesh->material->SetVec3("directionalLight.diffuse", glm::vec3(1.0f));
  cube->mesh->material->SetVec3("directionalLight.specular", glm::vec3(1.0f));

  cube->mesh->material->SetVec3("pointLight.ambient", glm::vec3(0.5f));
  cube->mesh->material->SetVec3("pointLight.diffuse", glm::vec3(1.0f));
  cube->mesh->material->SetVec3("pointLight.specular", glm::vec3(1.0f));

  /*cube->mesh->material->SetVec3("light.ambient", glm::vec3(1.0f));*/
  /*cube->mesh->material->SetVec3("light.specular", glm::vec3(1.0f));*/
  /*cube->mesh->material->SetFloat("light.constant", 1.0f);*/
  /*cube->mesh->material->SetFloat("light.linear", 0.14f);*/
  /*cube->mesh->material->SetFloat("light.quadratic", 0.07f);*/
  /*cube->mesh->material->SetVec3("light.spotDirection",*/
  /*                              glm::vec3(0.0f, -1.0f, 0.0f));*/
  /*cube->mesh->material->SetFloat("light.cutoff", glm::radians(45.0f));*/
  /*cube->mesh->material->SetFloat("light.outerCutoff", glm::radians(50.0f));*/
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

  cube->mesh->material->SetVec3("pointLight.position", lightPosWorldSpace);
  cube->mesh->material->SetVec3("light.position", lightPosWorldSpace);
  cube->mesh->material->SetVec3("cameraPosition", cameraWorldSpace);
  cube->mesh->material->SetFloat("globalFloat", globalFloat);
  cube->mesh->material->SetVec3("light.diffuse", lightColor);

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
