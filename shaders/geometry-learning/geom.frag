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
in VertexAttribute gs_vertexAttribute;

struct Camera {
    vec3 position;
};
uniform Camera camera;

uniform vec2 mousePosition;

void main() {
    vec3 color = vec3(0.0f);

    vec3 lightDir = normalize(vec3(0.5f));
    float diff = dot(gs_vertexAttribute.normal, lightDir);
    color = vec3(1.0f) * diff;

    FragColor = vec4(color, 1.0f);
}
