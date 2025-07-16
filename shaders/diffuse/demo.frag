#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

in vec3 fragPos;
in vec3 normal;
uniform vec3 lightPosition;

uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float globalFloat;

void main()
{
    vec3 lightDirection = normalize(lightPosition - fragPos);
    float diff = dot(normal, lightDirection);
    vec3 diffuse = diff * objectColor;
    FragColor = vec4(diffuse, 1.0f);
}
