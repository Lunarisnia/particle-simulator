#pragma once
#include <memory>
namespace Core {
class Component {
 public:
  template <class Type, typename... Args>
  static std::shared_ptr<Type> Create(Args... args);
};

template <class Type, typename... Args>
std::shared_ptr<Type> Component::Create(Args... args) {
  return std::make_shared<Type>(args...);
}
};  // namespace Core
