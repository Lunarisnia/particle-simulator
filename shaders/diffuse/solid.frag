#version 330 core
out vec4 FragColor;
struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};
uniform Material material;

void main()
{
    vec4 texColor = texture(material.diffuse, vertexAttribute.textureCoord);

    vec3 color = vec3(1.0f);
    FragColor = vec4(color, 1.0f);
}
