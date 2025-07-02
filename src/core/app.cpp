#include "core/app.hpp"
#include "core/input/input.hpp"
#include "core/renderer/renderer.hpp"
#include "core/window/window.hpp"
#include "core/world/world.hpp"

void Core::App::Init() {
  Window::Init();
  Renderer::Init();
  Input::Init();
  World::Init();
}

void Core::App::Run() {
  while (!Window::ShouldClose()) {
    Window::PollEvent();
    Input::PollMouse();
    World::Tick();

    Renderer::Render();

    Window::SwapBuffer();
    Renderer::Clear();
  }
}

Core::App::~App() {
  Window::Cleanup();
  Renderer::Cleanup();
  Input::Cleanup();
  World::Cleanup();
}
