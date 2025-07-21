#include "particle/primitive/primitive.hpp"
#include <memory>
#include "core/object/object.hpp"
#include "core/shader/shader.hpp"
#include "core/world/world.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

std::shared_ptr<Core::Object> Particle::Primitive::CreatePlane(
    const std::string& vertPath, const std::string& fragPath) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  Core::Shader shader{vertPath, fragPath};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  mesh->AddVertex(Core::VertexData{
      glm::vec3(-0.5f, 0.5f, 0.0f),
      glm::vec2(0.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(-0.5f, -0.5f, 0.0f),
      glm::vec2(0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(0.5f, -0.5f, 0.0f),
      glm::vec2(1.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(0.5f, 0.5f, 0.0f),
      glm::vec2(1.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddIndex(0);
  mesh->AddIndex(1);
  mesh->AddIndex(2);

  mesh->AddIndex(2);
  mesh->AddIndex(3);
  mesh->AddIndex(0);

  mesh->SetupMesh();

  Core::World::AddObject(obj);

  return obj;
}

std::shared_ptr<Core::Object> Particle::Primitive::CreateCube(
    const std::string& vertPath, const std::string& fragPath) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  Core::Shader shader{vertPath, fragPath};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  Core::VertexData vData;
  // Front face (z = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));  // Top right
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    vData.SetNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(0);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));  // Bottom right
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(1);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));  // Bottom left
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(2);

    mesh->AddIndex(0);
    mesh->AddIndex(2);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));  // Top left
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(3);
  }

  // Right face (x = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    vData.SetNormal(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(4);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(5);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(6);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(7);

    mesh->AddIndex(6);
    mesh->AddIndex(5);
  }

  // Left face (x = -0.5)
  {
    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(8);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(9);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(10);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(11);

    mesh->AddIndex(10);
    mesh->AddIndex(9);
  }

  // Back face (z = -0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, 0.0f, -1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(12);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(13);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(14);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(15);

    mesh->AddIndex(14);
    mesh->AddIndex(13);
  }

  // Up face (y = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(16);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(17);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(18);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(19);

    mesh->AddIndex(18);
    mesh->AddIndex(17);
  }

  // Down face (y = -0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(20);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(21);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(22);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(23);

    mesh->AddIndex(22);
    mesh->AddIndex(21);
  }

  mesh->SetupMesh();
  Core::World::AddObject(obj);

  return obj;
}
