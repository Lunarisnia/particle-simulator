#pragma once
#include <memory>
#include "core/object/object.hpp"
namespace Particle {
class Primitive {
 public:
  static std::shared_ptr<Core::Object> CreatePlane();
  static std::shared_ptr<Core::Object> CreateCube();
};
}  // namespace Particle
