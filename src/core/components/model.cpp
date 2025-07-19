#include "core/components/model.hpp"
#include <memory>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "core/components/mesh.hpp"

Core::Model::Model() {}

Core::Model::Model(const std::string& path) {}

Core::ComponentType Core::Model::GetType() { return ComponentType::MODEL; }

void Core::Model::Update() {}

std::vector<std::shared_ptr<Core::Mesh>>& Core::Model::GetMeshes() {
  return meshes;
}

void Core::Model::AddMesh(std::shared_ptr<Mesh>& mesh) {
  meshes.emplace_back(mesh);
}
