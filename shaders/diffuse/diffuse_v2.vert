#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;
    vec4 lightSpaceFragPos;

    mat3 TBN;
};
out VertexAttribute vertexAttribute;
out vec3 objectColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    objectColor = aColor;
    vertexAttribute.textureCoord = aTexCoord;
    vertexAttribute.normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    // vertexAttribute.normal = aNormal;
    vertexAttribute.fragPos = vec3(model * vec4(aPos, 1.0f));
    vertexAttribute.tangent = aTangent;
    vertexAttribute.lightSpaceFragPos = lightSpaceMatrix * vec4(vertexAttribute.fragPos, 1.0f);

    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0f)));
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0f)));
    vec3 B = cross(N, T);
    vertexAttribute.TBN = transpose(mat3(T, B, N));
}
