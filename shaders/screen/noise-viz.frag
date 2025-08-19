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

uniform samplerCube shadowTexture;

uniform float globalFloat;
uniform float globalFloat2;
uniform int currentFrame;
uniform vec2 resolution;

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

const uint k = 1103515245U;
vec3 hash33(uvec3 x)
{
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;

    return vec3(x) * (1.0 / float(0xffffffffU));
}

vec3 valueNoise() {
    vec2 uv = gl_FragCoord.xy / resolution;
    uv *= 20.0f;
    float x = step(0.5f, uv.x);
    float y = step(0.5f, uv.y);

    vec2 pc = uv * vec2(2.0f) - 0.5f;
    vec2 i = floor(pc) + 0.5f;
    // vec2 f = fract(uv);

    vec3 n1 = hash33(uvec3(i, 1.0f));
    vec3 n2 = hash33(uvec3(i + vec2(1.0f, 0.0f), 1.0f));
    vec3 n3 = hash33(uvec3(i + vec2(0.0f, 1.0f), 1.0f));
    vec3 n4 = hash33(uvec3(i + vec2(1.0f, 1.0f), 1.0f));

    vec2 f = smoothstep(0.0f, 1.0f, fract(pc));

    vec3 px1 = mix(n1, n2, f.x);
    vec3 px2 = mix(n3, n4, f.x);
    vec3 color = mix(px1, px2, f.y);
    return vec3(color.x);
}

void main()
{
    // vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
    // vec4 outline = texture(outlineTexture, vertexAttribute.textureCoord);
    // vec4 depth = texture(depthTexture, vertexAttribute.textureCoord);

    uvec3 p = uvec3(gl_FragCoord.xy, currentFrame);

    vec3 color = valueNoise();
    FragColor = vec4(color, 1.0f);
}
