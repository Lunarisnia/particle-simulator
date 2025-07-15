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
};

uniform Material material;
uniform Light light;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

vec3 calculateDirectionalLight(vec4 tex, vec4 specularHighlight) {
    vec3 ambient = directionalLight.ambient * tex.rgb;

    float diff = max(dot(normal, -directionalLight.direction), 0.0f);
    vec3 diffuse = directionalLight.diffuse * diff * tex.rgb;

    vec3 lightReflectionDir = reflect(directionalLight.direction, normal);
    vec3 viewDir = normalize(cameraPosition - fragPos);
    float specDiff = pow(max(dot(viewDir, lightReflectionDir), 0.0f), material.shininess);
    vec3 specular = directionalLight.specular * specDiff * specularHighlight.rgb;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(vec4 tex, vec4 specularHighlight) {
    vec3 ambient = pointLight.ambient * tex.rgb;

    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = pointLight.diffuse * diff * tex.rgb;

    vec3 lightReflectionDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(cameraPosition - fragPos);
    float specDiff = pow(max(dot(viewDir, lightReflectionDir), 0.0f), material.shininess);
    vec3 specular = pointLight.specular * specDiff * specularHighlight.rgb;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 tex = texture(material.diffuse, textureCoord);
    vec4 specularMap = texture(material.specular, textureCoord);
    vec4 emissionMap = texture(material.emission, textureCoord);

    vec3 color = calculateDirectionalLight(tex, specularMap);
    color += calculatePointLight(tex, specularMap);

    // Light
    // vec3 ambient = light.ambient * tex.rgb + (step(1.0f, vec3(1.0f) - specularMap.rgb) * emissionMap.rgb);
    //
    // vec3 lightDir = normalize(light.position - fragPos);
    // float theta = dot(lightDir, -light.spotDirection);
    // float epsilon = light.outerCutoff - light.cutoff;
    // float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
    //
    // float diff = max(dot(normal, lightDir), 0.0f);
    // vec3 diffuse = light.diffuse * diff * tex.rgb;
    //
    // vec3 cameraDir = normalize(cameraPosition - fragPos);
    // vec3 reflectionDir = reflect(-lightDir, normal);
    //
    // float specularDiff = pow(max(dot(cameraDir, reflectionDir), 0.0f), material.shininess);
    // vec3 specular = light.specular * specularDiff * specularMap.rgb;
    // diffuse *= intensity;
    // specular *= intensity;
    //
    // float fragDistance = length(light.position - fragPos);
    // float attenuation = 1.0f / (light.constant + light.linear * fragDistance + light.quadratic * fragDistance * fragDistance);
    // diffuse *= attenuation;
    // ambient *= attenuation;
    // specular *= attenuation;
    //
    // vec3 color = diffuse + ambient + specular;

    FragColor = vec4(color, 1.0f);
}
