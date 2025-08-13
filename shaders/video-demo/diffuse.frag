#version 330 core
layout(location = 0) out vec4 FragColor;

uniform float currentTime;
struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

struct Camera {
    vec3 position;
};
uniform Camera camera;

uniform vec2 mousePosition;

vec3 calculateDirectionalLight(vec3 lightPosition, VertexAttribute vertexAttribute) {
    vec3 color = vec3(0.0f);

    vec3 lightDir = normalize(lightPosition - vertexAttribute.fragPos);
    float diff = max(dot(vertexAttribute.normal, lightDir), 0.0f);
    vec3 diffuse = vec3(1.0f) * diff;

    vec3 viewDir = normalize(camera.position - vertexAttribute.fragPos);
    // vec3 halfDir = normalize(lightDir + viewDir);
    // float spec = pow(1.0f - max(0.0f, dot(halfDir, vertexAttribute.normal)), 0.4f);
    // vec3 specular = vec3(1.0f, 0.1f, 0.1f) * spec;

    color += diffuse;
    return color;
}

vec3 calculateDirectionalLightCell(vec3 lightPosition, VertexAttribute vertexAttribute) {
    vec3 color = vec3(0.0f);

    vec3 lightDir = normalize(lightPosition - vertexAttribute.fragPos);
    float diff = max(dot(vertexAttribute.normal, lightDir), 0.0f);
    diff = step(0.2f, diff);
    vec3 diffuse = vec3(1.0f) * diff;

    vec3 viewDir = normalize(camera.position - vertexAttribute.fragPos);
    // vec3 halfDir = normalize(lightDir + viewDir);
    // float spec = pow(1.0f - max(0.0f, dot(halfDir, vertexAttribute.normal)), 0.4f);
    // vec3 specular = vec3(1.0f, 0.1f, 0.1f) * spec;

    color += diffuse;
    return color;
}

void main() {
    vec2 nmc = mousePosition + 1.0f / 2.0f;
    vec2 uv = gl_FragCoord.xy / vec2(1280.0f, 768.0f);

    if (nmc.x > uv.x) {
        // Demo for diffuse light
        FragColor = vec4(calculateDirectionalLightCell(vec3(0.5f, 0.5f, 1.0f), vertexAttribute), 1.0f);
    } else {
        FragColor = vec4(calculateDirectionalLight(vec3(0.5f, 0.5f, 1.0f), vertexAttribute), 1.0f);
    }
}
