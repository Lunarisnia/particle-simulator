#include "core/app.hpp"
#include "core/window/window.hpp"

void Core::App::Run() { Window::Init(); }

Core::App::~App() { Window::Cleanup(); }
