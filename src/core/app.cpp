#include "core/app.hpp"
#include "core/input/input.hpp"
#include "core/renderer/renderer.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/time/time.hpp"
#include "core/window/window.hpp"
#include "core/world/world.hpp"
#include "core/static_light/static_light.hpp"

void Core::App::Init() {
  Window::Init();
  Renderer::Init();
  Input::Init();
  World::Init();
  StaticCamera::Init();
  StaticLight::Init();
}

// NOTE: why the hell does this exist???
void Core::App::Run() {
  while (!Window::ShouldClose()) {
    Time::Tick();
    Window::PollEvent();
    Input::PollMouse();
    World::Tick();

    Renderer::Render();

    Window::SwapBuffer();
    Renderer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

Core::App::~App() {
  Window::Cleanup();
  Renderer::Cleanup();
  Input::Cleanup();
  World::Cleanup();
  StaticCamera::Cleanup();
}
