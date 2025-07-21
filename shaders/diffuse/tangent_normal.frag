#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform float globalFloat;

void main()
{
    // vec3 lightDirection = normalize(lightPosition - fragPos);
    // float diff = dot(normal, lightDirection);
    // vec3 diffuse = diff * objectColor;
    FragColor = vec4(vertexAttribute.normal, 1.0f);
}
