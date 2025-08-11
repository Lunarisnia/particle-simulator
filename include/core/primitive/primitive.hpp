#pragma once
#include <memory>
#include <string>
#include "core/object/object.hpp"
namespace Core {
class Primitive {
 public:
  static std::shared_ptr<Core::Object> CreatePlane(const std::string& vertPath,
                                                   const std::string& fragPath);
  static std::shared_ptr<Core::Object> CreateCube(const std::string& vertPath,
                                                  const std::string& fragPath);
  static std::shared_ptr<Core::Object> CreateUVSphere(
      const std::string& vertPath, const std::string& fragPath,
      const std::string& name, int sectorCount = 36, int stackCount = 18,
      float radius = 0.5f);
};
}  // namespace Core
