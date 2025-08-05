#include "core/components/point_light.hpp"
#include <format>
#include <memory>
#include <string>
#include <vector>
#include "core/components/component.hpp"
#include "core/object/object.hpp"
#include "core/window/window.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

Core::PointLight::PointLight() {}

Core::ComponentType Core::PointLight::GetType() {
  return ComponentType::POINT_LIGHT;
}

void Core::PointLight::Update() {}

void Core::PointLight::SetMeshUniform(const std::string &uniform,
                                      std::shared_ptr<Core::Object> &object) {
  Core::Object *lightOwner = owner;
  object->mesh->material->SetVec3(std::format("{}.position", uniform),
                                  lightOwner->transform->GetWorldPosition());
  object->mesh->material->SetFloat(std::format("{}.farPlane", uniform),
                                   farPlane);
  object->mesh->material->SetVec3(std::format("{}.ambient", uniform), ambient);
  object->mesh->material->SetVec3(std::format("{}.diffuse", uniform), diffuse);
  object->mesh->material->SetVec3(std::format("{}.specular", uniform),
                                  specular);
  object->mesh->material->SetFloat(std::format("{}.constant", uniform),
                                   constant);
  object->mesh->material->SetFloat(std::format("{}.linear", uniform), linear);
  object->mesh->material->SetFloat(std::format("{}.quadratic", uniform),
                                   quadratic);
}

glm::mat4 Core::PointLight::getViewMatrix(glm::vec3 direction, glm::vec3 up) {
  glm::mat4 view(1.0f);
  view = glm::lookAt(owner->transform->position,
                     owner->transform->position + direction, up);
  return view;
}

glm::mat4 Core::PointLight::getProjectionMatrix() {
  glm::mat4 projection =
      glm::perspective(glm::radians(90.0f), 1024.0f / 1024.0f, 1.00f, farPlane);

  return projection;
}

std::vector<glm::mat4> Core::PointLight::GetCubeMapLightMatrix() {
  glm::mat4 shadowProj = getProjectionMatrix();
  std::vector<glm::mat4> shadowTransforms;
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj *
      getViewMatrix(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

  return shadowTransforms;
}
