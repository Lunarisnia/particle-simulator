#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
// This is essentially the same as view space matrix
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0f);
}
