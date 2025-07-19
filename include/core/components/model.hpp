#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/mesh.hpp"
namespace Core {
class Model : public Component {
 public:
  std::vector<std::shared_ptr<Mesh>> meshes;

 public:
  Core::ComponentType GetType() override;
  void Update() override;

  std::vector<std::shared_ptr<Mesh>>& GetMeshes();
  void AddMesh(std::shared_ptr<Mesh>& mesh);

 public:
  Model();
  Model(const std::string& path);
};
};  // namespace Core
