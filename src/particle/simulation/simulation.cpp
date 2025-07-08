#include "particle/simulation/simulation.hpp"
#include <_stdlib.h>
#include <memory>
#include "core/components/rigidbody2d.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/static_camera/static_camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "particle/primitive/primitive.hpp"

std::shared_ptr<Core::Object> Particle::Simulation::cube;

void Particle::Simulation::Init() {
  // TODO: fix the coordinate system
  Core::StaticCamera::transform->position.z = 1.0f;
  cube = Primitive::CreatePlane();
  cube->transform->scale = glm::vec3(0.2f);
  /*std::shared_ptr<Core::Rigidbody2D> rb =*/
  /*    cube->AddComponent<Core::Rigidbody2D>();*/
}

void Particle::Simulation::Update() {
  /*std::random_device rd;*/
  /*std::mt19937 gen(rd());*/
  /*std::uniform_real_distribution<float> distrib(-2.0f, 2.0f);*/
  /*std::uniform_real_distribution<float> distrib2(2.0f, 4.0f);*/

  if (Core::Input::GetMouseButton(0)) {
    // FIXME: Mouse position is not exactly accurate
    std::shared_ptr<Core::Object> obj = Primitive::CreatePlane();
    glm::vec4 res = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 wSpace = Core::Input::mouse.ToWorldPosition();

    obj->transform->scale = glm::vec3(0.005f);
    obj->transform->position = glm::vec3(wSpace.x, wSpace.y, 0.0f);
    obj->AddComponent<Core::Rigidbody2D>()->velocity = glm::vec2(0.1f, 2.0f);
  }
}
