#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float globalFloat;

void main()
{
    FragColor = vec4(objectColor, 1.0f);
}
