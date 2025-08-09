#include "editor/inspector/inspector.hpp"
#include "core/world/world.hpp"
#include "imgui.h"
void Editor::Inspector::Tick() {
  ImGui::Begin("Inspector");
  if (Core::World::selectedObject != nullptr) {
    ImGui::Text("Selected: %s", Core::World::selectedObject->name.c_str());
    ImGui::DragFloat3("Transform",
                      &Core::World::selectedObject->transform->position.x,
                      0.05f);
    ImGui::Checkbox("Is Active", &Core::World::selectedObject->isActive);
  } else {
    ImGui::Text("Selected: NONE");
  }
  ImGui::End();
}
