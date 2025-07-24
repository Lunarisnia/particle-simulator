#pragma once
#include <memory>
#include <vector>
#include "core/object/object.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Particle {
class Simulation {
 public:
  static float globalFloat;
  static float globalFloat2;
  static glm::vec3 lightColor;
  static std::vector<std::shared_ptr<Core::Object>> cubes;
  static std::vector<std::shared_ptr<Core::Object>> lightCubes;
  static std::vector<std::shared_ptr<Core::Object>> spotLightCubes;

 public:
  static void Init();
  static void Update();
};
}  // namespace Particle
