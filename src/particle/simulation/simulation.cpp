#include "particle/simulation/simulation.hpp"
#include <memory>
#include "core/object/object.hpp"
#include "core/world/world.hpp"
void Particle::Simulation::Init() {
  std::shared_ptr<Core::Object> cube = std::make_shared<Core::Object>();
  Core::Shader shader{"./shaders/diffuse/diffuse.vert",
                      "./shaders/diffuse/diffuse.frag"};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = cube->AddComponent<Core::Mesh>(material);
  mesh->AddVertex(Core::VertexData{
      0.5f,
      -0.5f,
      0.0f,
  });
  mesh->AddVertex(Core::VertexData{
      0.5f,
      0.5f,
      0.0f,
  });
  mesh->AddVertex(Core::VertexData{
      -0.5f,
      -0.5f,
      0.0f,
  });
  mesh->AddVertex(Core::VertexData{
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

  Core::World::AddObject(cube);
}

void Particle::Simulation::Update() {}
