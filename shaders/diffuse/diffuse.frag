#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPosition;

uniform vec3 objectColor;

uniform float globalFloat;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
    vec4 tex = texture(material.diffuse, textureCoord);
    vec4 specularMap = texture(material.specular, textureCoord);
    vec4 emissionMap = texture(material.emission, textureCoord);

    // Light
    float fragDistance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * fragDistance + light.quadratic * fragDistance * fragDistance);

    vec3 ambient = light.ambient * tex.rgb + (step(1.0f, vec3(1.0f) - specularMap.rgb) * emissionMap.rgb);

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * tex.rgb;

    vec3 cameraDir = normalize(cameraPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specularDiff = pow(max(dot(cameraDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = light.specular * specularDiff * specularMap.rgb;

    diffuse *= attenuation;
    ambient *= attenuation;
    specular *= attenuation;

    vec3 color = diffuse + ambient + specular;
    FragColor = vec4(color, 1.0f);
}
