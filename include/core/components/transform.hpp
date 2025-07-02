#pragma once
#include "core/components/component.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class Transform : public Component {
 public:
  glm::vec3 position;
};
}  // namespace Core
