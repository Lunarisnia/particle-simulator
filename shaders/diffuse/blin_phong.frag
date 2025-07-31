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

float calculateShadow(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).x;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

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

    float shadow = calculateShadow(vertexAttribute.lightSpaceFragPos);
    color += (1.0f - shadow) * (diffuse + specular);
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
