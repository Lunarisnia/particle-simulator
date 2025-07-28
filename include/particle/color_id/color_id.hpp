#pragma once
#include <vector>
#include "glm/ext/vector_float3.hpp"
namespace Particle {
class ColorID {
 private:
  static size_t nextColorIndex;
  static std::vector<glm::vec3> colors;

 public:
  static glm::vec3 Generate();
};
};  // namespace Particle
