#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/mesh.hpp"
#include "core/components/transform.hpp"
namespace Core {
class Object {
 public:
  std::string name;
  std::shared_ptr<Transform> transform;
  std::shared_ptr<Mesh> mesh;

 private:
  std::vector<std::shared_ptr<Component>> components;

 public:
  void Update();
  template <class Type, typename... Args>
  std::shared_ptr<Type> AddComponent(Args... args);

  template <class Type>
  std::shared_ptr<Type> GetComponent();

 public:
  Object();
};

template <class Type, typename... Args>
std::shared_ptr<Type> Object::AddComponent(Args... args) {
  std::shared_ptr<Type> component = Component::Create<Type>(args...);

  component->owner = this;
  components.emplace_back(component);

  return component;
}

template <class Type>
std::shared_ptr<Type> Object::GetComponent() {
  if constexpr (!std::is_base_of<Component, Type>::value) {
    return nullptr;
  }

  std::shared_ptr<Type> temp = Component::Create<Type>();
  for (std::shared_ptr<Component> &comp : components) {
    if (temp->GetType() == comp->GetType()) {
      return std::dynamic_pointer_cast<Type>(comp);
    }
  }
  return nullptr;
}
};  // namespace Core
