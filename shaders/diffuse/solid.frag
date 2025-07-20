#version 330 core
out vec4 FragColor;

in vec2 textureCoord;

in vec3 fragPos;
in vec3 normal;

uniform sampler2D tex;
// uniform vec3 lightPosition;

// uniform vec3 cameraPosition;
// uniform vec3 objectColor;
// uniform float globalFloat;

void main()
{
    vec4 texColor = texture(tex, textureCoord);

    vec3 color = vec3(1.0f);
    FragColor = vec4(texColor.rgb, 1.0f);
}
