#pragma once
#include <memory>
#include "core/object/object.hpp"
namespace Particle {
class Simulation {
 public:
  static float globalFloat;
  static std::shared_ptr<Core::Object> cube;
  static std::shared_ptr<Core::Object> lightCube;
  static std::shared_ptr<Core::Object> groundCube;

 public:
  static void Init();
  static void Update();
};
}  // namespace Particle
