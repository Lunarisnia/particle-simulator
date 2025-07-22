#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight[2];

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};
uniform Material material;

struct Camera {
    vec3 position;
};
uniform Camera camera;

vec3 calculatePointLight(PointLight light, vec3 diffuseTexture, vec3 specularTexture, vec3 normal) {
    // All calculation are done in tangent space
    vec3 color = vec3(0.0f);

    vec3 lightDir = vertexAttribute.TBN * normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, normal));
    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 viewDir = vertexAttribute.TBN * normalize(camera.position - vertexAttribute.fragPos);
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(0.0f, dot(halfVector, normal)), 16.0f);
    vec3 specular = light.specular * spec * specularTexture;

    color += diffuse + specular;
    return color;
}

void main()
{
    vec4 diffuseTexture = texture(material.diffuse, vertexAttribute.textureCoord);
    vec4 specularTexture = texture(material.specular, vertexAttribute.textureCoord);
    vec4 normalTexture = texture(material.normal, vertexAttribute.textureCoord);
    normalTexture = normalTexture * 2.0f - 1.0f;

    vec3 color = vec3(0.0f);
    for (int i = 0; i < 1; i++) {
        color += calculatePointLight(pointLight[i], diffuseTexture.rgb, specularTexture.rgb, normalTexture.rgb);
    }

    float gamma = 2.2f;
    color = pow(color, vec3(1.0f / gamma));
    FragColor = vec4(color, 1.0f);
}
