#pragma once
#include <memory>
#include <vector>
#include "core/object/object.hpp"
namespace Core {
class World {
 public:
  static std::vector<std::shared_ptr<Object>> hierarchy;

 public:
  static void Init();
  static void Tick();
  static void Cleanup();

  static void AddObject(std::shared_ptr<Object> object);
};
}  // namespace Core
