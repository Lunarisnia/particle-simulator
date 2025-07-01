#pragma once
#include "core/input/mouse.hpp"
namespace Core {
class Input {
 public:
  static Mouse mouse;

 public:
  static void Init();
  static bool GetKey(int key);
  static bool GetMouseButton(int key);
  static void PollMouse();
  static void Cleanup();
};
};  // namespace Core
