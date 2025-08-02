#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;
    vec4 lightSpaceFragPos;

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

uniform sampler2D shadowMap;

float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.04f, 0.05 * (1.0f - dot(lightDir, normal)));
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            float closestDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).x;
            shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0f;
    if (projCoords.z > 1.0f) {
        shadow = 0.0f;
    }

    return shadow;
}

vec3 calculatePointLight(PointLight light, vec3 diffuseTexture, vec3 specularTexture, vec3 normal) {
    // All calculation are done in tangent space
    vec3 color = vec3(0.0f);

    vec3 lightDir = vertexAttribute.TBN * normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, normal));
    // Cell shader
    // diff *= smoothstep(0.5f, 0.599f, diff);
    vec3 diffuse = light.diffuse * diff * diffuseTexture;

    vec3 viewDir = vertexAttribute.TBN * normalize(camera.position - vertexAttribute.fragPos);
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(0.0f, dot(halfVector, normal)), 32.0f);
    vec3 specular = light.specular * spec * specularTexture;

    // vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    float shadow = calculateShadow(vertexAttribute.lightSpaceFragPos, lightDir, normal);
    // color += (1.0f - shadow) * (diffuse + specular);
    color += mix(vec3(0.0f), diffuse + specular, 1.0f - shadow);
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
