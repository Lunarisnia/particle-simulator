#pragma once
namespace Core {
class Time {
 public:
  static float timeSinceStartup;
  static float deltaTime;
  static int frameSinceStartup;

 public:
  static void Tick();
};
}  // namespace Core
