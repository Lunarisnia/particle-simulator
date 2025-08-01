#pragma once
#include "core/components/mesh.hpp"
namespace Core {
class Procedural {
 public:
  static void GeneratePlane(std::shared_ptr<Mesh>& mesh);
  static void GenerateCube(std::shared_ptr<Mesh>& mesh);
  static void GenerateUVSphere(std::shared_ptr<Mesh>& mesh, int sectorCount,
                               int stackCount, float radius);
};
};  // namespace Core
