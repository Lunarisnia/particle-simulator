#include "particle/simulation/simulation.hpp"
#include <memory>
#include "core/components/rigidbody2d.hpp"
#include "core/input/input.hpp"
#include "core/object/object.hpp"
#include "core/components/rigidbody2d.hpp"
#include "glm/ext/vector_float3.hpp"
#include "particle/primitive/primitive.hpp"

std::shared_ptr<Core::Object> Particle::Simulation::cube;

void Particle::Simulation::Init() {
  cube = Primitive::CreatePlane();
  std::shared_ptr<Core::Rigidbody2D> rb =
      cube->AddComponent<Core::Rigidbody2D>();
}

void Particle::Simulation::Update() {
  std::shared_ptr<Core::Rigidbody2D> rb =
      cube->GetComponent<Core::Rigidbody2D>();
  if (Core::Input::GetMouseButton(0)) {
    // FIXME: Mouse position is not exactly accurate
    std::shared_ptr<Core::Object> obj = Primitive::CreatePlane();
    glm::vec2 ndcSpace = Core::Input::mouse.ToNDCPosition();

    obj->transform->scale = glm::vec3(0.1f);
    obj->transform->position = glm::vec3(ndcSpace, -0.5f);
    obj->AddComponent<Core::Rigidbody2D>()->velocity = glm::vec2(1.0f, 1.0f);
  }
}
