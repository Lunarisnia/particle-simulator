#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D screenTexture;

vec3 blackAndWhite(vec3 color) {
    color = vec3((color.r + color.g + color.b) / 3.0f);
    return color;
}

void main()
{
    vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
    FragColor = vec4(screen.rgb, 1.0f);
}
