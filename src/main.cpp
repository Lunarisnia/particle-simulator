#include <exception>
#include <iostream>
#include "core/app.hpp"

int main() {
  try {
    Core::App app;
    app.Init();
    app.Run();
  } catch (const std::exception &error) {
    std::cout << error.what() << std::endl;
    return 1;
  }
}
