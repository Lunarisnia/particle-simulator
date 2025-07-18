#pragma once
#include "core/components/component.hpp"
namespace Core {
// TODO: Implement cpp file for model, it is basically a class that can contain
// a vector of mesh and have it be handled by the renderer
class Model : public Component {
 public:
  Core::ComponentType GetType() override;
  void Update() override;

 public:
  Model();
};
};  // namespace Core
