#include "core/components/mesh.hpp"
#include "core/vertex/vertex.hpp"
#include "glad/glad.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
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
                        (void *)(sizeof(glm::vec3)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (void *)(sizeof(glm::vec3) + sizeof(glm::vec2)));
  glEnableVertexAttribArray(2);
}

void Core::Mesh::BindVertexArray() { glBindVertexArray(VAO); }

void Core::Mesh::AddVertex(Core::VertexData vertex) {
  vertice.emplace_back(vertex);
}

void Core::Mesh::AddIndex(int index) { indice.emplace_back(index); }

int Core::Mesh::GetIndiceLength() { return indice.size(); }

Core::ComponentType Core::Mesh::GetType() { return ComponentType::MESH; }
