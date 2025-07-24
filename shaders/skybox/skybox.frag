#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform samplerCube cubeTexture;

void main()
{
    // TODO: Do environment mapping for the reflection
    vec4 skybox = texture(cubeTexture, vertexAttribute.textureCoord);
    FragColor = vec4(skybox.rgb, 1.0f);
}
