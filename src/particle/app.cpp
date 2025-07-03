#include "particle/app.hpp"
#include <exception>
#include "core/app.hpp"
#include "core/input/input.hpp"
#include "core/renderer/renderer.hpp"
#include "core/window/window.hpp"
#include "core/world/world.hpp"
#include "particle/simulation/simulation.hpp"
Particle::App::App() {
  core = Core::App();
  try {
    core.Init();
    Simulation::Init();
  } catch (const std::exception& error) {
    throw error;
  }
}

void Particle::App::Run() {
  try {
    while (!Core::Window::ShouldClose()) {
      Core::Window::PollEvent();
      Core::Input::PollMouse();
      Simulation::Update();
      Core::World::Tick();

      Core::Renderer::Render();

      Core::Window::SwapBuffer();
      Core::Renderer::Clear();
    }
  } catch (const std::exception& error) {
    throw error;
  }
}
