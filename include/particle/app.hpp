#pragma once
#include <memory>
#include "core/app.hpp"
#include "core/framebuffer/framebuffer.hpp"
#include "core/object/object.hpp"
namespace Particle {
class App {
 private:
  Core::App core;
  std::shared_ptr<Core::Framebuffer> framebuffer;
  std::shared_ptr<Core::Object> renderPlane;

 private:
  void initFramebuffer();
  void initRenderPlane();

 public:
  void Run();

 public:
  App();
};
}  // namespace Particle
