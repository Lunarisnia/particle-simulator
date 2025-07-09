#version 330 core
out vec4 FragColor;

in vec3 normal;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 color = ambient * objectColor;
    FragColor = vec4(color, 1.0f);
}
