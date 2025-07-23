#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

struct VertexAttribute {
    vec3 textureCoord;
};
out VertexAttribute vertexAttribute;

void main()
{
    vec3 pos = aPos * 2.0f;
    vertexAttribute.textureCoord = pos;
    gl_Position = vec4(pos.xy, 1.0f, 1.0f);
}
