#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPosition;
uniform float globalFloat;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D normalMap;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 spotDirection;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

#define NUMBER_OF_POINT_LIGHT 2
#define NUMBER_OF_SPOT_LIGHT 1

uniform Material material;
uniform Light light;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[NUMBER_OF_POINT_LIGHT];
uniform SpotLight spotLight[NUMBER_OF_SPOT_LIGHT];

vec3 calculateDirectionalLight(vec4 tex, vec4 specularHighlight, vec3 normal, float roughness) {
    vec3 ambient = directionalLight.ambient * tex.rgb;

    float diff = max(dot(normal, -directionalLight.direction), 0.0f);
    vec3 diffuse = directionalLight.diffuse * diff * tex.rgb;

    vec3 lightReflectionDir = reflect(directionalLight.direction, normal);
    vec3 viewDir = normalize(cameraPosition - fragPos);
    float specDiff = pow(max(dot(viewDir, lightReflectionDir), 0.0f), roughness);
    vec3 specular = directionalLight.specular * specDiff * specularHighlight.rgb;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(vec4 tex, vec4 specularHighlight, PointLight pLight, vec3 normal, float roughness) {
    vec3 ambient = pLight.ambient * tex.rgb;

    vec3 lightDir = normalize(pLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = pLight.diffuse * diff * tex.rgb;

    vec3 lightReflectionDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(cameraPosition - fragPos);
    float specDiff = pow(max(dot(viewDir, lightReflectionDir), 0.0f), roughness);
    vec3 specular = pLight.specular * specDiff * specularHighlight.rgb;

    float fragDistance = length(pLight.position - fragPos);
    float attenuation = 1.0f / (pLight.constant + pLight.linear * fragDistance + pLight.quadratic * fragDistance * fragDistance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(vec4 tex, vec4 specularHighlight, SpotLight sLight, vec3 normal, float roughness) {
    vec3 ambient = sLight.ambient * tex.rgb;

    vec3 lightDir = normalize(sLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = sLight.diffuse * diff * tex.rgb;

    vec3 lightReflectionDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(cameraPosition - fragPos);
    float specDiff = pow(max(dot(viewDir, lightReflectionDir), 0.0f), roughness);
    vec3 specular = sLight.specular * specDiff * specularHighlight.rgb;

    float theta = dot(lightDir, -sLight.direction);
    float epsilon = sLight.outerCutoff - sLight.cutoff;
    float intensity = clamp((theta - sLight.outerCutoff) / epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    // ambient *= intensity;
    specular *= intensity;

    float fragDistance = length(sLight.position - fragPos);
    float attenuation = 1.0f / (sLight.constant + sLight.linear * fragDistance + sLight.quadratic * fragDistance * fragDistance);

    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;

    return diffuse + specular;
}

void main()
{
    vec4 tex = texture(material.diffuse, textureCoord);
    vec4 specularMap = texture(material.specular, textureCoord);
    vec4 emissionMap = texture(material.emission, textureCoord);
    vec4 normalMap = texture(material.normalMap, textureCoord);
    normalMap = normalize(normalMap * 2.0f - 1.0f);

    vec3 color = calculateDirectionalLight(tex, specularMap, normalMap.rgb, material.shininess);
    for (int i = 0; i < NUMBER_OF_POINT_LIGHT; i++) {
        color += calculatePointLight(tex, specularMap, pointLight[i], normalMap.rgb, material.shininess);
    }

    for (int i = 0; i < NUMBER_OF_SPOT_LIGHT; i++) {
        color += calculateSpotLight(tex, specularMap, spotLight[i], normalMap.rgb, material.shininess);
    }

    color += (step(1.0f, vec3(1.0f) - specularMap.rgb) * emissionMap.rgb);

    FragColor = vec4(color, 1.0f);
}
