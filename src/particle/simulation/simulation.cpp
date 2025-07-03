#include "particle/simulation/simulation.hpp"
#include <memory>
#include "core/object/object.hpp"
#include "core/time/time.hpp"
#include "particle/primitive/primitive.hpp"

std::shared_ptr<Core::Object> Particle::Simulation::cube;

void Particle::Simulation::Init() { cube = Primitive::CreatePlane(); }

void Particle::Simulation::Update() {
  cube->transform->position.x = (float)cos(Core::Time::timeSinceStartup);
  cube->transform->position.y = (float)sin(Core::Time::timeSinceStartup);
}
