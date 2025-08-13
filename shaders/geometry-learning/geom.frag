#version 330 core
layout(location = 0) out vec4 FragColor;

uniform float currentTime;
struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

struct Camera {
    vec3 position;
};
uniform Camera camera;

uniform vec2 mousePosition;

void main() {
    FragColor = vec4(1.0f);
}
