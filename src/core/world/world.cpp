#include "core/world/world.hpp"
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/renderer/renderer.hpp"
#include "core/shader/shader.hpp"
#include "core/vertex/vertex.hpp"

std::vector<std::shared_ptr<Core::Object>> Core::World::hierarchy;

void Core::World::Init() {
  std::shared_ptr<Object> cube = std::make_shared<Object>();
  Shader shader{"./shaders/diffuse/diffuse.vert",
                "./shaders/diffuse/diffuse.frag"};
  std::shared_ptr<Material> material = std::make_shared<Material>(shader);
  std::shared_ptr<Mesh> mesh = cube->AddComponent<Mesh>(material);
  mesh->AddVertex(VertexData{
      0.5f,
      -0.5f,
      0.0f,
  });
  mesh->AddVertex(VertexData{
      0.5f,
      0.5f,
      0.0f,
  });
  mesh->AddVertex(VertexData{
      -0.5f,
      -0.5f,
      0.0f,
  });
  mesh->AddVertex(VertexData{
      -0.5f,
      0.5f,
      0.0f,
  });
  mesh->AddIndex(0);
  mesh->AddIndex(1);
  mesh->AddIndex(2);
  mesh->AddIndex(3);
  mesh->AddIndex(1);
  mesh->AddIndex(2);

  mesh->SetupMesh();

  AddObject(cube);
}

void Core::World::Tick() {
  for (std::shared_ptr<Object> &object : hierarchy) {
    object->Update();
  }
}

void Core::World::AddObject(std::shared_ptr<Object> object) {
  hierarchy.emplace_back(object);

  std::shared_ptr<Mesh> mesh = object->GetComponent<Mesh>();
  if (mesh != nullptr) {
    Renderer::AddToRenderQueue(mesh);
    /*Voxedit::Hierarchy::AddObjectToHierarchy(object->id, object);*/
  }
}

void Core::World::Cleanup() {}
