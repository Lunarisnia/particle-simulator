#version 330 core
layout(location = 0) out vec4 FragColor;

in GS_Out {
    vec3 fragPos;
    vec3 normal;
} gs_in;

uniform float currentTime;

vec3 colorNormal(vec3 lightPosition) {
    vec3 color;
    vec3 lightDir = normalize(lightPosition - gs_in.fragPos);
    float diff = max(0.0f, dot(lightDir, gs_in.normal));

    color = mix(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), diff);
    return color;
}

void main() {
    vec3 color = vec3(1.0f, 1.0f, 0.0f);
    FragColor = vec4(color, 1.0f);
}
