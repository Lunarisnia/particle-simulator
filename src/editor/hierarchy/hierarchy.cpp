#include "editor/hierarchy/hierarchy.hpp"
#include <cstddef>
#include <print>
#include "core/world/world.hpp"
#include "imgui.h"

size_t Editor::Hierarchy::selectedItemIndex = 0;

void Editor::Hierarchy::Tick() {
  ImGui::ShowDemoWindow();
  ImGui::Begin("Hierarchy");
  if (ImGui::BeginListBox("Objects")) {
    for (size_t i = 0; i < Core::World::hierarchy.size(); i++) {
      const bool isSelected = (selectedItemIndex == i);
      if (ImGui::Selectable(Core::World::hierarchy[i]->name.c_str(),
                            isSelected)) {
        std::println("Hello");
        // TODO: set selected object pointer from here
      }
    }
    ImGui::EndListBox();
  }
  ImGui::End();
}
