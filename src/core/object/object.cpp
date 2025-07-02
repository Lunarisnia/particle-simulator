#include "core/object/object.hpp"
#include <iostream>
#include "core/components/transform.hpp"

Core::Object::Object() { transform = AddComponent<Transform>(); }

void Core::Object::Update() { std::cout << transform->position.x << std::endl; }
