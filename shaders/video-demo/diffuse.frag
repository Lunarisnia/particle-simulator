#version 330 core
layout(location = 0) out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

uniform vec2 mousePosition;

void main() {
    FragColor = vec4(vec3(mousePosition, 0.0f), 1.0f);
}
