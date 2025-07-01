#pragma once
#include "core/shader/shader.hpp"
namespace Core {
class Material {
 public:
  Shader shader;

 public:
  void Use();

 public:
  Material(Shader shader);
};
}  // namespace Core
