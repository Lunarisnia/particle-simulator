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

    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLight[2];
uniform float globalFloat;

vec3 calculatePointLight(PointLight light, vec3 brightColor, vec3 shadowColor) {
    vec3 color = vec3(0.0f);
    vec3 lightDir = normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, vertexAttribute.normal));
    vec3 diffuse = mix(shadowColor, brightColor, step(globalFloat, diff));
    // vec3 diffuse = brightColor * diff;

    color += diffuse;
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
