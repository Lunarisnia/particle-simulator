#include "core/components/spot_light.hpp"
#include <format>
#include <memory>
#include <string>
#include "core/components/component.hpp"
#include "core/object/object.hpp"

Core::SpotLight::SpotLight() {}

Core::ComponentType Core::SpotLight::GetType() {
  return ComponentType::SPOT_LIGHT;
}

void Core::SpotLight::Update() {}

void Core::SpotLight::SetMeshUniform(const std::string &uniform,
                                     std::shared_ptr<Core::Object> &object) {
  Core::Object *lightOwner = GetOwner();
  object->mesh->material->SetVec3(std::format("{}.position", uniform),
                                  lightOwner->transform->GetWorldPosition());
  object->mesh->material->SetVec3(std::format("{}.direction", uniform),
                                  direction);
  object->mesh->material->SetVec3(std::format("{}.ambient", uniform), ambient);
  object->mesh->material->SetVec3(std::format("{}.diffuse", uniform), diffuse);
  object->mesh->material->SetVec3(std::format("{}.specular", uniform),
                                  specular);
  object->mesh->material->SetFloat(std::format("{}.constant", uniform),
                                   constant);
  object->mesh->material->SetFloat(std::format("{}.linear", uniform), linear);
  object->mesh->material->SetFloat(std::format("{}.quadratic", uniform),
                                   quadratic);
  object->mesh->material->SetFloat(std::format("{}.cutoff", uniform), cutoff);
  object->mesh->material->SetFloat(std::format("{}.outerCutoff", uniform),
                                   outerCutoff);
}
