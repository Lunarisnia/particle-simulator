#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

struct VertexAttribute {
    vec2 textureCoord;
};
out VertexAttribute vertexAttribute;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexAttribute.textureCoord = aTexCoord;
}
