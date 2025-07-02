#include "core/world/world.hpp"
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"

std::vector<std::shared_ptr<Core::Object>> Core::World::hierarchy;

void Core::World::Init() {
  std::shared_ptr<Object> cube = std::make_shared<Object>();
  cube->AddComponent<Mesh>();

  AddObject(cube);
}

void Core::World::Tick() {
  for (std::shared_ptr<Object> &object : hierarchy) {
    object->Update();
  }
}

void Core::World::AddObject(std::shared_ptr<Object> object) {
  hierarchy.emplace_back(object);
}

void Core::World::Cleanup() {}
