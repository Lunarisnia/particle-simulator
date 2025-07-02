#include "core/object/object.hpp"
#include "core/components/component.hpp"
#include "core/components/transform.hpp"

Core::Object::Object() { transform = AddComponent<Transform>(); }

void Core::Object::Update() {
  for (std::shared_ptr<Component>& component : components) {
    component->Update();
  }
}
