#pragma once
#include <memory>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/mesh.hpp"
namespace Core {
class Object {
 public:
  // TODO: Transform
  std::shared_ptr<Mesh> mesh;

 private:
  std::vector<std::shared_ptr<Component>> components;

 public:
  template <class Component, typename... Args>
  void AddComponent(Args... args);

 public:
  Object();
};

template <class Type, typename... Args>
void Object::AddComponent(Args... args) {
  std::shared_ptr<Type> component = Component::Create<Type>(args...);

  components.emplace_back(component);
}
};  // namespace Core
