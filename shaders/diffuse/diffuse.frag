#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPosition;

uniform vec3 objectColor;

uniform float globalFloat;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D objectTexture;

void main()
{
    // TODO: Create light data structure for each light type
    vec3 ambient = light.ambient * material.ambient;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 cameraDir = normalize(cameraPosition - fragPos);
    vec3 reflectionDir = reflect(-lightDir, normal);

    float specularDiff = pow(max(dot(cameraDir, reflectionDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (specularDiff * material.specular);

    vec4 tex = texture(objectTexture, textureCoord);
    vec3 color = (diffuse + ambient + specular) * vec3(tex);
    FragColor = vec4(color, 1.0f);
}
