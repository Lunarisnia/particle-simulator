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
std::shared_ptr<Core::Object> Particle::Simulation::groundCube;

const std::string vertexPath = "./shaders/diffuse/diffuse.vert";
const std::string diffuseFrag = "./shaders/diffuse/diffuse.frag";
const std::string lightFrag = "./shaders/light/light.frag";

void Particle::Simulation::Init() {
  /*glm::vec3 a = glm::vec3(1.0f, 0.0f, 0.0f);*/
  /*glm::vec3 b = glm::vec3(0.5f, 0.5f, 0.5f);*/
  /*std::println("{}", glm::dot(a, b));*/

  Core::StaticCamera::transform->position.z = 1.0f;

  lightCube = Primitive::CreateCube(vertexPath, lightFrag);
  lightCube->transform->position = glm::vec3(1.5f, 1.5f, -1.5f);

  cube = Primitive::CreateCube(vertexPath, diffuseFrag);
  cube->transform->position.z = -1.0f;
  cube->mesh->material->SetVec3("objectColor", glm::vec3(0.7f, 0.3f, 0.4f));
  cube->mesh->material->SetVec3("lightColor", glm::vec3(1.0f));

  groundCube = Primitive::CreateCube(vertexPath, diffuseFrag);
  groundCube->transform->position = glm::vec3(0.0f, -1.5f, 0.0f);
  groundCube->transform->scale = glm::vec3(200.0f, 1.0f, 100.0f);
  groundCube->mesh->material->SetVec3("objectColor", glm::vec3(0.3f));
  groundCube->mesh->material->SetVec3("lightColor", glm::vec3(1.0f));
}

void Particle::Simulation::Update() {
  glm::vec3 lightPosWorldSpace =
      lightCube->transform->GetTransformMatrix() *
      glm::vec4(lightCube->transform->position, 1.0f);
  glm::vec3 cameraWorldSpace =
      Core::StaticCamera::transform->GetTransformMatrix() *
      glm::vec4(Core::StaticCamera::transform->position, 1.0f);

  cube->mesh->material->SetVec3("lightPosition", lightPosWorldSpace);
  cube->mesh->material->SetVec3("cameraPosition", cameraWorldSpace);
  cube->mesh->material->SetFloat("globalFloat", globalFloat);
  groundCube->mesh->material->SetVec3("lightPosition", lightPosWorldSpace);
  groundCube->mesh->material->SetVec3("cameraPosition", cameraWorldSpace);
  groundCube->mesh->material->SetFloat("globalFloat", globalFloat);

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
