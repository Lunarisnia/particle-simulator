#pragma once
#include <memory>
#include <string>
#include "core/object/object.hpp"
#include "core/shader/shader.hpp"
namespace Core {
class Primitive {
 public:
  static std::shared_ptr<Core::Object> CreatePlane(Shader shader);
  static std::shared_ptr<Core::Object> CreateCube(const std::string& vertPath,
                                                  const std::string& fragPath);
  static std::shared_ptr<Core::Object> CreateUVSphere(Shader shader,
                                                      const std::string& name,
                                                      int sectorCount = 36,
                                                      int stackCount = 18,
                                                      float radius = 0.5f);
};
}  // namespace Core
