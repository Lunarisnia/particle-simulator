#version 330 core
out vec4 FragColor;
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

void main()
{
    vec4 texColor = texture(material.diffuse, vertexAttribute.textureCoord);

    vec3 color = calculatePointLight(pointLight[0], texColor.rgb, normalize(vertexAttribute.normal));
    FragColor = vec4(color, 1.0f);
}
