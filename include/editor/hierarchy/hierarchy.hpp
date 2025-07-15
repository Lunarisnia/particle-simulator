#pragma once
#include <cstddef>
namespace Editor {
class Hierarchy {
 private:
  static size_t selectedItemIndex;

 public:
  static void Tick();
};
};  // namespace Editor
