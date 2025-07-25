#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ModelColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};
uniform Material material;

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight[2];

struct Camera {
    vec3 position;
};
uniform Camera camera;

uniform float globalFloat;
uniform float globalFloat2;

vec3 calculatePointLight(PointLight light, vec3 diffuseTexture, vec3 normal) {
    // All calculation are done in tangent space
    diffuseTexture *= vec3(0.8f);
    vec3 color = vec3(0.0f);

    vec3 lightDir = normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, normal));
    // Cell shader
    // diff *= smoothstep(0.230f, 0.239f, diff);
    vec3 diffuseColor = light.diffuse * diff * diffuseTexture;
    // NOTE: returning the texture directly result in this flat color
    vec3 diffuse = mix(vec3(0.8f) * diffuseTexture, diffuseTexture, smoothstep(0.230f, 0.239f, diff));

    vec3 viewDir = normalize(camera.position - vertexAttribute.fragPos);
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(0.0f, dot(halfVector, normal)), 1.0f);
    vec3 specular = light.specular * spec * diffuseTexture;

    color += diffuse;
    return color;
}

float near = 0.01f;
float far = 1.5f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    vec4 texColor = texture(material.diffuse, vertexAttribute.textureCoord);

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    ModelColor = vec4(mix(vec3(1.0f), vec3(0.0f), depth), 1.0f);

    vec3 color = calculatePointLight(pointLight[0], texColor.rgb, normalize(vertexAttribute.normal));
    FragColor = vec4(color, 1.0f);
}
