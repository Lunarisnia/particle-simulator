#pragma once
#include <memory>
#include "core/object/object.hpp"
namespace Particle {
class Simulation {
 public:
  static std::shared_ptr<Core::Object> cube;

 public:
  static void Init();
  static void Update();
};
}  // namespace Particle
