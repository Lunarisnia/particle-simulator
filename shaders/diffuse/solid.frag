#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

in vec3 fragPos;
in vec3 normal;
// uniform vec3 lightPosition;

// uniform vec3 cameraPosition;
// uniform vec3 objectColor;
// uniform float globalFloat;

void main()
{
    vec3 color = vec3(1.0f);
    FragColor = vec4(color, 1.0f);
}
