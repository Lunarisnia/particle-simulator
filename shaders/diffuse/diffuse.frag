#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec3 result = objectColor * lightColor;
    FragColor = vec4(result, 1.0f);
}
