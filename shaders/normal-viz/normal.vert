#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_Out {
    vec3 fragPos;
    vec3 normal;
} vs_out;

void main()
{
    gl_Position = view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vs_out.normal = normalize(mat3(transpose(inverse(view * model))) * aNormal);
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0f));
}
