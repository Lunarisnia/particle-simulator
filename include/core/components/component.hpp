#pragma once
#include <memory>
namespace Core {

class Object;

enum ComponentType {
  TRANSFORM,
  MESH,
};

class Component {
 public:
  Object *owner;

 public:
  Object *GetOwner();

  virtual ComponentType GetType();
  virtual void Update();

  template <class Type, typename... Args>
  static std::shared_ptr<Type> Create(Args... args);
};

template <class Type, typename... Args>
std::shared_ptr<Type> Component::Create(Args... args) {
  return std::make_shared<Type>(args...);
}
};  // namespace Core
