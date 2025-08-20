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

    vec2 texSize = vec2(2.0f);
    vec2 pc = uv * texSize - 0.5f;
    vec2 i = floor(pc) + 0.5f;

    vec3 n1 = hash33(uvec3(i, 1.0f));
    vec3 n2 = hash33(uvec3(i + vec2(1.0f, 0.0f) / texSize, 1.0f));
    vec3 n3 = hash33(uvec3(i + vec2(0.0f, 1.0f) / texSize, 1.0f));
    vec3 n4 = hash33(uvec3(i + vec2(1.0f, 1.0f) / texSize, 1.0f));

    vec2 f = smoothstep(0.0f, 1.0f, fract(pc));

    vec3 px1 = mix(n1, n2, f.x);
    vec3 px2 = mix(n3, n4, f.x);
    vec3 color = mix(px1, px2, f.y);
    return vec3(color.x);
}

vec3 perlinNoise() {
    vec2 uv = gl_FragCoord.xy / resolution;
    uv *= 20.0f;

    vec3 i = floor(vec3(uv, 0.0f));
    vec3 f = fract(vec3(uv, 0.0f));

    // Fade curve (smooth interpolation)
    // vec3 u = f * f * (3.0 - 2.0 * f);
    vec3 u = smoothstep(0.0f, 1.0f, f);

    // --- Corners on z = 0 plane ---
    // TODO: need better hash than integer hash
    float dot000 = dot(hash33(uvec3(i + vec3(0.0, 0.0, 0.0))), f - vec3(0.0, 0.0, 0.0));
    float dot100 = dot(hash33(uvec3(i + vec3(1.0, 0.0, 0.0))), f - vec3(1.0, 0.0, 0.0));
    float dot010 = dot(hash33(uvec3(i + vec3(0.0, 1.0, 0.0))), f - vec3(0.0, 1.0, 0.0));
    float dot110 = dot(hash33(uvec3(i + vec3(1.0, 1.0, 0.0))), f - vec3(1.0, 1.0, 0.0));

    float mixX00 = mix(dot000, dot100, u.x);
    float mixX10 = mix(dot010, dot110, u.x);
    float mixY0 = mix(mixX00, mixX10, u.y);

    // --- Corners on z = 1 plane ---
    float dot001 = dot(hash33(uvec3(i + vec3(0.0, 0.0, 1.0))), f - vec3(0.0, 0.0, 1.0));
    float dot101 = dot(hash33(uvec3(i + vec3(1.0, 0.0, 1.0))), f - vec3(1.0, 0.0, 1.0));
    float dot011 = dot(hash33(uvec3(i + vec3(0.0, 1.0, 1.0))), f - vec3(0.0, 1.0, 1.0));
    float dot111 = dot(hash33(uvec3(i + vec3(1.0, 1.0, 1.0))), f - vec3(1.0, 1.0, 1.0));

    float mixX01 = mix(dot001, dot101, u.x);
    float mixX11 = mix(dot011, dot111, u.x);
    float mixY1 = mix(mixX01, mixX11, u.y);

    // --- Final interpolation along z ---
    return vec3(mix(mixY0, mixY1, u.z));
}

void main()
{
    // vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
    // vec4 outline = texture(outlineTexture, vertexAttribute.textureCoord);
    // vec4 depth = texture(depthTexture, vertexAttribute.textureCoord);

    uvec3 p = uvec3(gl_FragCoord.xy, currentFrame);

    vec3 color = perlinNoise();
    FragColor = vec4(color, 1.0f);
}
