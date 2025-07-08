#pragma once
namespace Editor {
class Editor {
 public:
  static void Init();
  static void Update();
  static void NewFrame();
  static void Render();
  static void Cleanup();
};
};  // namespace Editor
