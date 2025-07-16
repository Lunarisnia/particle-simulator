#pragma once
#include "core/components/component.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class SpotLight : public Component {
 public:
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float cutoff;
  float outerCutoff;
  float constant;
  float linear;
  float quadratic;

 public:
  ComponentType GetType() override;
  void Update() override;

  void SetMeshUniform(const std::string &uniform,
                      std::shared_ptr<Core::Object> &object);

 public:
  SpotLight();
};
};  // namespace Core
