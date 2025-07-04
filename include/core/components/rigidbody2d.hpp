#pragma once
#include <memory>
#include "core/components/component.hpp"
#include "core/components/transform.hpp"
#include "glm/ext/vector_float2.hpp"
namespace Core {
class Rigidbody2D : public Component {
 private:
  std::shared_ptr<Transform> objectTransform;

 public:
  float mass = 1.0f;
  glm::vec2 velocity;
  glm::vec2 force;

 public:
  void Init();
  Core::ComponentType GetType() override;
  void Update() override;

 public:
  Rigidbody2D();
};
}  // namespace Core
