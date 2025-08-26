#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

struct VertexAttribute {
    vec3 fragPos;
    vec2 textureCoord;
};
out VertexAttribute vertexAttribute;

void main()
{
    gl_Position = vec4(aPos.xy * 2.0f, 0.0f, 1.0);
    vertexAttribute.textureCoord = aTexCoord;
    vertexAttribute.fragPos = vec3(aPos.xy * 2.0f, 0.0f);
}
