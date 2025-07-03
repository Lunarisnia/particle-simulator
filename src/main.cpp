#include <exception>
#include <iostream>
#include "particle/app.hpp"

int main() {
  try {
    Particle::App app;
    app.Run();
  } catch (const std::exception &error) {
    std::cout << error.what() << std::endl;
    return 1;
  }
}
