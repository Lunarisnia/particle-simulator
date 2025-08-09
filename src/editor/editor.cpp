#include "core/input/input.hpp"
#include "particle/simulation/simulation.hpp"
#include "editor/editor.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "core/static_camera/static_camera.hpp"
#include "core/window/window.hpp"
#include "editor/hierarchy/hierarchy.hpp"
#include "editor/inspector/inspector.hpp"
#include "imgui.h"

void Editor::Editor::Init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(Core::Window::GetWindow(), true);
  ImGui_ImplOpenGL3_Init();
}

void Editor::Editor::NewFrame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Editor::Editor::Update() {
  // TODO: Abstract this out better
  ImGui::Begin("Debug");
  ImGui::DragFloat3("Camera Position",
                    &Core::StaticCamera::transform->position.x, 0.05f);
  ImGui::DragFloat3("Camera Lookat", &Core::StaticCamera::front.x, 0.05f);
  ImGui::DragFloat3("Diffuse Light Color", &Particle::Simulation::lightColor.x,
                    0.05f);
  ImGui::DragFloat("Global Shader Float", &Particle::Simulation::globalFloat,
                   0.001f);
  ImGui::DragFloat("Global Shader Float2", &Particle::Simulation::globalFloat2,
                   0.001f);
  ImGui::Text("Mouse Coord: (X:%f, Y:%f)",
              (Core::Input::mouse.ToNDCPosition().x + 1.0f) / 2.0f,
              (Core::Input::mouse.ToNDCPosition().y + 1.0f) / 2.0f);
  ImGui::End();
  Hierarchy::Tick();
  Inspector::Tick();
}

void Editor::Editor::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Editor::Cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
