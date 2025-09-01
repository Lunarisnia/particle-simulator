#include "core/primitive/primitive.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include "core/object/object.hpp"
#include "core/procedural/procedural.hpp"
#include "core/shader/shader.hpp"
#include "core/world/world.hpp"

std::shared_ptr<Core::Object> Core::Primitive::CreatePlane(
    Core::Shader shader) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  Core::Procedural::GeneratePlane(mesh);
  Core::World::AddObject(obj);
  return obj;
}

std::shared_ptr<Core::Object> Core::Primitive::CreatePlane(Core::Shader shader,
                                                           int div) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  Core::Procedural::GenerateComplexPlane(mesh, div);
  Core::World::AddObject(obj);
  return obj;
}

std::shared_ptr<Core::Object> Core::Primitive::CreateCube(
    const std::string& vertPath, const std::string& fragPath) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  Core::Shader shader{vertPath, fragPath};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  Core::Procedural::GenerateCube(mesh);
  Core::World::AddObject(obj);
  return obj;
}

std::shared_ptr<Core::Object> Core::Primitive::CreateUVSphere(
    Core::Shader shader, const std::string& name, int sectorCount,
    int stackCount, float radius) {
  std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> mesh = obj->AddComponent<Core::Mesh>(material);
  obj->mesh = mesh;
  Core::Procedural::GenerateUVSphere(mesh, sectorCount, stackCount, radius);
  Core::World::AddObject(obj);
  return obj;
}
