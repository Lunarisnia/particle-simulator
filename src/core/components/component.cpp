#include "core/components/component.hpp"

void Core::Component::Update() {}

Core::Object *Core::Component::GetOwner() { return owner; }

Core::ComponentType Core::Component::GetType() {
  return ComponentType::TRANSFORM;
}
