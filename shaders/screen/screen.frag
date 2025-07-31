#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

uniform sampler2D screenTexture;
uniform sampler2D outlineTexture;

uniform sampler2D depthTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

uniform sampler2D shadowTexture;

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
        sampleTexture[i] = texture(outlineTexture, vertexAttribute.textureCoord + offsets[i]).rgb;
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
    vec4 shadowTex = texture(shadowTexture, vertexAttribute.textureCoord);
    // vec4 outline = texture(outlineTexture, vertexAttribute.textureCoord);
    //
    // vec4 depth = texture(depthTexture, vertexAttribute.textureCoord);
    // float kernel[9] = float[](
    //         0.5f, 1.0f, 0.5f,
    //         1.0f, 2.0f, 1.0f,
    //         0.5f, 1.0f, 0.5f
    //     );
    // vec3 blur = useKernel3x3(kernel, 1.0f / 1000.0f);

    // vec3 color = mix(screen.rgb, vec3(0.0f), outline.rgb);
    FragColor = vec4(screen.rgb, 1.0f);
}
