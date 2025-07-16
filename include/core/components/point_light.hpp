#pragma once
#include <memory>
#include <string>
#include "core/components/component.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class PointLight : public Component {
 public:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

 public:
  ComponentType GetType() override;
  void Update() override;

  void SetMeshUniform(const std::string &uniform,
                      std::shared_ptr<Core::Object> &object);

 public:
  PointLight();
};
};  // namespace Core
