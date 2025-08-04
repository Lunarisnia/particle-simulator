#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Core {
class PointLight : public Component {
 public:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

 private:
  glm::mat4 getViewMatrix(glm::vec3 direction, glm::vec3 up);
  glm::mat4 getProjectionMatrix();

 public:
  ComponentType GetType() override;
  void Update() override;

  void SetMeshUniform(const std::string &uniform,
                      std::shared_ptr<Core::Object> &object);

  std::vector<glm::mat4> GetCubeMapLightMatrix();

 public:
  PointLight();
};
};  // namespace Core
