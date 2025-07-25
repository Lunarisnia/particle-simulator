#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

uniform float globalFloat;
uniform float globalFloat2;

vec3 blackAndWhite(vec3 color) {
    color = vec3((color.r + color.g + color.b) / 3.0f);
    return color;
}

vec3 useKernel3x3(float kernel[9], float offset) {
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
        sampleTexture[i] = texture(depthTexture, vertexAttribute.textureCoord + offsets[i]).rgb;
    }
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++) {
        color += sampleTexture[i] * kernel[i];
    }

    return color;
}

void main()
{
    vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
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
    // vec3 sobelX = useKernel3x3(sobelXKernel, 1.0f / 300.0f);
    // vec3 sobelY = useKernel3x3(sobelYKernel, 1.0 / 300.0f);
    // TODO: Find out how to do anti aliasing
    float edgeDetectionKernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
        );
    float blurKernel[9] = float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        );
    vec3 edge = useKernel3x3(edgeDetectionKernel, 1.0f / 900.0f);
    vec3 blur = useKernel3x3(blurKernel, 1.0f / 300.0f);
    vec3 outline = edge;
    float f = dot(outline, vec3(1.0f)) / 3.0f;

    vec3 color = mix(screen.rgb, vec3(0.2f, 0.12f, 0.11f), step(0.5, f));
    FragColor = vec4(color, 1.0f);
}
