#include "editor/hierarchy/hierarchy.hpp"
#include <cstddef>
#include "core/world/world.hpp"
#include "imgui.h"

size_t Editor::Hierarchy::selectedItemIndex = 0;

void Editor::Hierarchy::Tick() {
  ImGui::Begin("Hierarchy");

  if (ImGui::BeginListBox("Objects")) {
    for (size_t i = 0; i < Core::World::hierarchy.size(); i++) {
      const bool isSelected = (selectedItemIndex == i);
      if (ImGui::Selectable(Core::World::hierarchy[i]->name.c_str(),
                            isSelected)) {
        Core::World::selectedObject = Core::World::hierarchy[i];
        selectedItemIndex = i;
      }
    }
    ImGui::EndListBox();
  }
  ImGui::End();
}
