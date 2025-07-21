#include "core/components/mesh.hpp"
#include "core/vertex/vertex.hpp"
#include "glad/glad.h"
#include <cstddef>
#include <memory>

Core::Mesh::Mesh() {}

Core::Mesh::Mesh(std::shared_ptr<Material> material) : material(material) {}

void Core::Mesh::Update() {}

void Core::Mesh::SetupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertice.size(),
               vertice.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indice.size(),
               indice.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)offsetof(VertexData, textureCoord));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)offsetof(VertexData, normal));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Core::Mesh::BindVertexArray() { glBindVertexArray(VAO); }

void Core::Mesh::AddVertex(Core::VertexData vertex) {
  vertice.emplace_back(vertex);
}

void Core::Mesh::AddIndex(unsigned int index) { indice.emplace_back(index); }

int Core::Mesh::GetIndiceLength() { return indice.size(); }

Core::VertexData *Core::Mesh::GetVertex(size_t i) {
  if (i >= vertice.size()) {
    return nullptr;
  }

  return &vertice.at(i);
}

Core::ComponentType Core::Mesh::GetType() { return ComponentType::MESH; }
