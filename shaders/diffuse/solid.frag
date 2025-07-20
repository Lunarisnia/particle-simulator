#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

in vec3 fragPos;
in vec3 normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
};

uniform Material material;
// uniform vec3 lightPosition;

// uniform vec3 cameraPosition;
// uniform vec3 objectColor;
// uniform float globalFloat;

void main()
{
    vec4 texColor = texture(material.diffuse, textureCoord);

    vec3 color = vec3(1.0f);
    FragColor = vec4(texColor.rgb, 1.0f);
}
