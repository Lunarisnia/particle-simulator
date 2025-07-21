#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
};
out VertexAttribute vertexAttribute;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexAttribute.textureCoord = aTexCoord;
    vertexAttribute.normal = aNormal;
    vertexAttribute.fragPos = vec3(model * vec4(aPos, 1.0f));
}
