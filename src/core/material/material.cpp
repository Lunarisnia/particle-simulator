#include "core/material/material.hpp"
#include "core/shader/shader.hpp"

Core::Material::Material(Core::Shader shader) : shader(shader) {}

void Core::Material::Use() { shader.Use(); }
