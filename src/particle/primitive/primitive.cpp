#include "particle/primitive/primitive.hpp"
#include <memory>
#include "core/object/object.hpp"
#include "core/world/world.hpp"

std::shared_ptr<Core::Object> Particle::Primitive::CreatePlane() {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  Core::Shader shader{"./shaders/diffuse/diffuse.vert",
                      "./shaders/diffuse/diffuse.frag"};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
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

  Core::World::AddObject(obj);

  return obj;
}
