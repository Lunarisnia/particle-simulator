#pragma once
namespace Editor {
class Inspector {
 public:
  static int octave;
  static float persistence;
  static float lacunarity;

 public:
  static void Tick();
};
};  // namespace Editor
