#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, vertexAttribute.textureCoord);
}
