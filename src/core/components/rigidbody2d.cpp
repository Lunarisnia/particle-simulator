#include "core/components/rigidbody2d.hpp"
#include "core/components/component.hpp"
#include "core/components/transform.hpp"
#include "core/object/object.hpp"
#include "core/physics/physics_world.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

Core::Rigidbody2D::Rigidbody2D() {}

void Core::Rigidbody2D::Init() {
  Core::Object *o = owner;
  if (o->GetComponent<Core::Transform>() == nullptr) {
    throw std::runtime_error(
        "object need to have a transform component attached for rigidbody2d to"
        "work");
  }
  objectTransform = o->GetComponent<Core::Transform>();
}

Core::ComponentType Core::Rigidbody2D::GetType() {
  return Core::ComponentType::RIGIDBODY_2D;
}

void Core::Rigidbody2D::Update() {
  Init();
  float timeStep = 1.0f / 60.0f;
  force = mass * Core::PhysicsWorld::gravity;
  velocity += force / mass * timeStep;
  objectTransform->position += glm::vec3(velocity * timeStep, 0.0f);
}
