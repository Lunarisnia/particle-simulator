#pragma once
#include <memory>
#include "core/object/object.hpp"
namespace Particle {
class Primitive {
 public:
  static std::shared_ptr<Core::Object> CreatePlane(const std::string& vertPath,
                                                   const std::string& fragPath);
  static std::shared_ptr<Core::Object> CreateCube(const std::string& vertPath,
                                                  const std::string& fragPath);
};
}  // namespace Particle
