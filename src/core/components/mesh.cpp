#include "core/components/mesh.hpp"
#include "core/vertex/vertex.hpp"
#include "glad/glad.h"
#include <memory>

Core::Mesh::Mesh(std::shared_ptr<Material> material) : material(material) {}

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

  glVertexAttribPointer(0, sizeof(VertexData), GL_FLOAT, GL_FALSE,
                        sizeof(VertexData), (void *)0);
  glEnableVertexAttribArray(0);
}

void Core::Mesh::AddVertex(Core::VertexData vertex) {
  vertice.emplace_back(vertex);
}

void Core::Mesh::AddIndex(int index) { indice.emplace_back(index); }
