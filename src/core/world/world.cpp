#include "core/world/world.hpp"
#include <memory>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/renderer/renderer.hpp"

std::vector<std::shared_ptr<Core::Object>> Core::World::hierarchy;
std::shared_ptr<Core::Object> Core::World::selectedObject;

void Core::World::Init() {}

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
