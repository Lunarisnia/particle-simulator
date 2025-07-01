#include "core/app.hpp"
#include "core/window/window.hpp"

void Core::App::Init() { Window::Init(); }

void Core::App::Run() {
  while (!Window::ShouldClose()) {
    Window::PollEvent();

    Window::SwapBuffer();
  }
}

Core::App::~App() { Window::Cleanup(); }
