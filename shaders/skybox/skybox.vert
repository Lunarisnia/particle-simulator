#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

struct VertexAttribute {
    vec3 textureCoord;
};
out VertexAttribute vertexAttribute;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertexAttribute.textureCoord = aPos * 2.0f;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos * 2.0f, 1.0f);
    gl_Position = pos.xyww;
}
