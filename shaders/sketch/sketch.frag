#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 DepthColor;
layout(location = 2) out vec4 NormalColor;
layout(location = 3) out vec4 ModelColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;
in vec3 objectColor;

struct PointLight {
    vec3 position;
    float farPlane;

    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLight[2];
uniform float globalFloat;
uniform samplerCube shadowCubeMap;

float calculateShadow(vec3 fragPos, vec3 lightPos, vec3 lightDir, float farPlane, vec3 normal) {
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(shadowCubeMap, fragToLight).x;
    closestDepth *= farPlane;
    float currentDepth = length(fragToLight);
    // check whether current frag pos is in shadow
    float bias = max(0.04f, 0.05 * (1.0f - dot(lightDir, normal)));
    // float bias = 0.05f;
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    return shadow;
}

vec3 calculatePointLight(PointLight light, vec3 brightColor, vec3 shadowColor) {
    vec3 color = vec3(0.0f);
    vec3 lightDir = normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, vertexAttribute.normal));
    vec3 diffuse = mix(shadowColor, brightColor, step(0.510f, diff));
    // vec3 diffuse = brightColor * diff;

    float bias = 0.05f;
    float shadow = calculateShadow(vertexAttribute.fragPos, light.position, lightDir, light.farPlane, vertexAttribute.normal);
    color += mix(vec3(0.0f), diffuse, 1.0f - shadow);
    // color += diffuse;
    return color;
}

float near = 0.01f;
float far = 100.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    vec3 color = calculatePointLight(pointLight[0], vec3(1.0f), vec3(0.0f));

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration

    ModelColor = vec4(objectColor, 1.0f);
    NormalColor = vec4(vertexAttribute.normal, 1.0f);
    DepthColor = vec4(vec3(depth), 1.0f);

    FragColor = vec4(color, 1.0f);
}
