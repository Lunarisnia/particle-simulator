#include "core/components/point_light.hpp"
#include <format>
#include <memory>
#include <string>
#include "core/components/component.hpp"
#include "core/object/object.hpp"

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
