#pragma once
#include "core/app.hpp"
namespace Particle {
class App {
 private:
  Core::App core;

 public:
  void Run();

 public:
  App();
};
}  // namespace Particle
