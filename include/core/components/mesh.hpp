#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "core/components/component.hpp"
#include "core/material/material.hpp"
#include "core/vertex/vertex.hpp"
namespace Core {
class Mesh : public Component {
 private:
  unsigned int VAO, VBO, EBO;
  std::vector<VertexData> vertice;
  std::vector<unsigned int> indice;

 public:
  std::shared_ptr<Material> material;
  void Update() override;
  ComponentType GetType() override;
  void SetupMesh();

  void BindVertexArray();
  void AddVertex(VertexData vertex);
  void AddIndex(unsigned int index);
  int GetIndiceLength();
  int GetVertexLength();
  VertexData *GetVertex(size_t i);

 public:
  Mesh();
  Mesh(std::shared_ptr<Material> material);
};
};  // namespace Core
