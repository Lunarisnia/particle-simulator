#include "editor/inspector/inspector.hpp"
#include "core/world/world.hpp"
#include "imgui.h"

int Editor::Inspector::octave = 0;
float Editor::Inspector::persistence = 0.5f;
float Editor::Inspector::lacunarity = 2.0f;

void Editor::Inspector::Tick() {
  ImGui::Begin("Inspector");
  if (Core::World::selectedObject != nullptr) {
    ImGui::Text("Selected: %s", Core::World::selectedObject->name.c_str());
    ImGui::DragFloat3("Transform",
                      &Core::World::selectedObject->transform->position.x,
                      0.05f);
    ImGui::Checkbox("Is Active", &Core::World::selectedObject->isActive);
    ImGui::DragInt("Octave", &octave, 0, 16);
    ImGui::DragFloat("Persistence", &persistence, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Lacunarity", &lacunarity, 0.01f, 0.0f, 256.0f);
  } else {
    ImGui::Text("Selected: NONE");
  }
  ImGui::End();
}
