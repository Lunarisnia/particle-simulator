#include "particle/simulation/simulation.hpp"
#include <iostream>
#include <memory>
#include "core/components/rigidbody2d.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/components/rigidbody2d.hpp"
#include "core/static_camera/static_camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "particle/primitive/primitive.hpp"

std::shared_ptr<Core::Object> Particle::Simulation::cube;

void Particle::Simulation::Init() {
  // TODO: fix the coordinate system
  Core::StaticCamera::transform->position.z = 1.0f;
  cube = Primitive::CreatePlane();
  cube->transform->scale = glm::vec3(0.5f);
  /*std::shared_ptr<Core::Rigidbody2D> rb =*/
  /*    cube->AddComponent<Core::Rigidbody2D>();*/
}

void Particle::Simulation::Update() {
  /*std::shared_ptr<Core::Rigidbody2D> rb =*/
  /*    cube->GetComponent<Core::Rigidbody2D>();*/
  /*glm::vec3 wSpace = Core::Input::mouse.ToWorldPosition();*/
  /*cube->transform->position.x = wSpace.x;*/
  /*cube->transform->position.y = wSpace.y;*/
  if (Core::Input::GetMouseButton(0)) {
    // FIXME: Mouse position is not exactly accurate
    std::shared_ptr<Core::Object> obj = Primitive::CreatePlane();
    glm::vec3 wSpace = Core::Input::mouse.ToWorldPosition();
    std::cout << wSpace.x << " : " << wSpace.y << std::endl;

    obj->transform->scale = glm::vec3(0.1f);
    obj->transform->position = glm::vec3(wSpace.x, wSpace.y, 0.0f);
    obj->AddComponent<Core::Rigidbody2D>()->velocity = glm::vec2(1.0f, 1.0f);
  }
}
