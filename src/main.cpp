#include "GLFW/glfw3.h"
#define GLFW_INCLUDE_NONE
#include <iostream>

int main() {
  std::cout << "Hello, World" << std::endl;
  if (!glfwInit()) {
    std::cout << "failed to initialize";
  }

  glfwTerminate();
}
