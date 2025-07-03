#pragma once
namespace Core {
class Time {
 public:
  static float timeSinceStartup;
  static float deltaTime;

 public:
  static void Tick();
};
}  // namespace Core
