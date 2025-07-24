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

uniform samplerCube skyboxTexture;

struct Camera {
    vec3 position;
};
uniform Camera camera;

void main() {
    vec3 viewDir = normalize(camera.position - vertexAttribute.fragPos);
    vec3 reflectionDir = reflect(-viewDir, normalize(vertexAttribute.normal));

    vec4 skybox = texture(skyboxTexture, reflectionDir);
    FragColor = vec4(skybox.rgb, 1.0f);
}
