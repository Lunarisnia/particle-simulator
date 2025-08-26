#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D depthTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

uniform float globalFloat;
uniform float globalFloat2;

vec3 useKernel3x3(float kernel[9], float offset, sampler2D tex) {
    vec2 offsets[9] = vec2[](
            vec2(-offset, offset),
            vec2(0.0f, offset),
            vec2(offset, offset),
            vec2(-offset, 0.0f),
            vec2(0.0f, 0.0f),
            vec2(offset, 0.0f),
            vec2(-offset, -offset),
            vec2(0.0f, -offset),
            vec2(offset, -offset)
        );

    vec3 sampleTexture[9];
    for (int i = 0; i < 9; i++) {
        sampleTexture[i] = texture(tex, vertexAttribute.textureCoord + offsets[i]).rgb;
    }
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++) {
        color += sampleTexture[i] * kernel[i];
    }

    return color;
}

void main()
{
    float sobelXKernel[9] = float[](
            1, 0, -1,
            2, 0, -2,
            1, 0, -1
        );
    float sobelYKernel[9] = float[](
            1, 2, 1,
            0, 0, 0,
            -1, -2, -1
        );
    float edgeDetectionKernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
        );

    vec3 colorSobelX = useKernel3x3(sobelXKernel, 1.0f / 900.0f, colorTexture);
    vec3 colorSobelY = useKernel3x3(sobelYKernel, 1.0f / 900.0f, colorTexture);

    vec3 normalSobelX = useKernel3x3(sobelXKernel, 1.0f / 900.0f, normalTexture);
    vec3 normalSobelY = useKernel3x3(sobelYKernel, 1.0f / 900.0f, normalTexture);

    vec3 depthSobelX = useKernel3x3(sobelXKernel, 1.0f / 900.0f, depthTexture);
    vec3 depthSobelY = useKernel3x3(sobelYKernel, 1.0f / 900.0f, depthTexture);

    // vec3 outline = clamp(vec3(length(normalSobelX) + length(normalSobelY)) + vec3(length(colorSobelX) + length(colorSobelY)), 0.0f, 1.0f);
    vec3 outline = clamp(vec3(length(normalSobelX + colorSobelX) + length(normalSobelY + colorSobelY)), 0.0f, 1.0f);
    float f = dot(outline, vec3(1.0f)) / 3.0f;

    vec4 tex = texture(depthTexture, vertexAttribute.textureCoord);

    vec3 color = mix(vec3(0.0f), vec3(1.0f), step(0.5, f));
    FragColor = vec4(color, 1.0f);
}
