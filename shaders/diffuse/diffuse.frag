#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float globalFloat;

void main()
{
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5f;
    vec3 cameraDir = normalize(cameraPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specularDiff = pow(max(dot(cameraDir, reflectionDir), 0.0f), 32);
    vec3 specular = specularDiff * lightColor;

    vec3 color = (diffuse + ambient + specular) * objectColor;
    FragColor = vec4(color, 1.0f);
}
