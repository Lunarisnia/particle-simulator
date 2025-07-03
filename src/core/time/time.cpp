#include "core/time/time.hpp"
#include "GLFW/glfw3.h"

float Core::Time::timeSinceStartup = 0.0f;
float Core::Time::deltaTime = 0.0f;

void Core::Time::Tick() {
  const float currentTime = (float)glfwGetTime();

  deltaTime = currentTime - timeSinceStartup;
  timeSinceStartup = currentTime;
}
