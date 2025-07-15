#pragma once
#include <memory>
#include "core/object/object.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Particle {
class Simulation {
 public:
  static float globalFloat;
  static glm::vec3 lightColor;
  static std::shared_ptr<Core::Object> cube;
  static std::shared_ptr<Core::Object> lightCube;

 public:
  static void Init();
  static void Update();
};
}  // namespace Particle
