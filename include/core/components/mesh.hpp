#pragma once
#include <memory>
#include <vector>
#include "core/components/component.hpp"
#include "core/material/material.hpp"
#include "core/vertex/vertex.hpp"
namespace Core {
class Mesh : public Component {
 private:
  unsigned int VAO, VBO, EBO;
  std::shared_ptr<Material> material;
  std::vector<VertexData> vertice;
  std::vector<int> indice;

 public:
  void SetupMesh();

  void AddVertex(VertexData vertex);
  void AddIndex(int index);

 public:
  Mesh();
  Mesh(std::shared_ptr<Material> material);
};
};  // namespace Core
