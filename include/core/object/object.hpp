#pragma once
#include <memory>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/mesh.hpp"
#include "core/components/transform.hpp"
namespace Core {
class Object {
 public:
  std::shared_ptr<Transform> transform;
  std::shared_ptr<Mesh> mesh;

 private:
  std::vector<std::shared_ptr<Component>> components;

 public:
  void Update();
  template <class Type, typename... Args>
  std::shared_ptr<Type> AddComponent(Args... args);

 public:
  Object();
};

template <class Type, typename... Args>
std::shared_ptr<Type> Object::AddComponent(Args... args) {
  std::shared_ptr<Type> component = Component::Create<Type>(args...);

  components.emplace_back(component);

  return component;
}
};  // namespace Core
