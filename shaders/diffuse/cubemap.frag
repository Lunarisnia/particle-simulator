#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

uniform samplerCube cubeTexture;

void main()
{
    vec4 cube = texture(cubeTexture, vertexAttribute.fragPos);
    FragColor = vec4(cube.rgb, 1.0f);
}
