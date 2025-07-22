#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D screenTexture;

void main()
{
    vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
    FragColor = vec4(screen.rgb, 1.0f);
}
