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
  std::shared_ptr<Core::Framebuffer> edgeDetectionFramebuffer;
  std::shared_ptr<Core::Framebuffer> shadowMapFramebuffer;
  std::shared_ptr<Core::Object> renderPlane;
  std::shared_ptr<Core::Object> edgeDetectionPlane;

 private:
  void initFramebuffer();
  void initRenderPlane();
  void edgeDetectionPass();

 public:
  void Run();

 public:
  App();
};
}  // namespace Particle
