#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
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

#define PI 3.141592653589793238462

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

vec3 hash(vec3 p) // this hash is not production ready, please
{ // replace this by something better
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
            dot(p, vec3(269.5, 183.3, 246.1)),
            dot(p, vec3(113.5, 271.9, 124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float perlinNoise(in vec3 x)
{
    // grid
    vec3 p = floor(x);
    vec3 w = fract(x);

    // quintic interpolant
    vec3 u = w * w * w * (w * (w * 6.0 - 15.0) + 10.0);

    // gradients
    vec3 ga = hash(p + vec3(0.0, 0.0, 0.0));
    vec3 gb = hash(p + vec3(1.0, 0.0, 0.0));
    vec3 gc = hash(p + vec3(0.0, 1.0, 0.0));
    vec3 gd = hash(p + vec3(1.0, 1.0, 0.0));
    vec3 ge = hash(p + vec3(0.0, 0.0, 1.0));
    vec3 gf = hash(p + vec3(1.0, 0.0, 1.0));
    vec3 gg = hash(p + vec3(0.0, 1.0, 1.0));
    vec3 gh = hash(p + vec3(1.0, 1.0, 1.0));

    // projections
    float va = dot(ga, w - vec3(0.0, 0.0, 0.0));
    float vb = dot(gb, w - vec3(1.0, 0.0, 0.0));
    float vc = dot(gc, w - vec3(0.0, 1.0, 0.0));
    float vd = dot(gd, w - vec3(1.0, 1.0, 0.0));
    float ve = dot(ge, w - vec3(0.0, 0.0, 1.0));
    float vf = dot(gf, w - vec3(1.0, 0.0, 1.0));
    float vg = dot(gg, w - vec3(0.0, 1.0, 1.0));
    float vh = dot(gh, w - vec3(1.0, 1.0, 1.0));

    // interpolation
    return va +
        u.x * (vb - va) +
        u.y * (vc - va) +
        u.z * (ve - va) +
        u.x * u.y * (va - vb - vc + vd) +
        u.y * u.z * (va - vc - ve + vg) +
        u.z * u.x * (va - vb - ve + vf) +
        u.x * u.y * u.z * (-va + vb + vc - vd + ve - vf - vg + vh);
}

float fbm(vec3 position, int n, float persistence, float lacunarity) {
    float frequency = 1.0f;
    float amplitude = 0.5f;
    float total = 0.0f;
    float normalization = 0.0f;
    for (int i = 0; i < n; i++) {
        float noiseValue = perlinNoise(position * frequency);
        total += noiseValue * amplitude;
        normalization += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;

    return total;
}

void main()
{
    // vec4 screen = texture(screenTexture, vertexAttribute.textureCoord);
    // vec4 outline = texture(outlineTexture, vertexAttribute.textureCoord);
    // vec4 depth = texture(depthTexture, vertexAttribute.textureCoord);

    uvec3 p = uvec3(gl_FragCoord.xy, currentFrame);

    // Initial noise sample position offset and scaled by uniform variables
    vec2 uv = gl_FragCoord.xy / resolution;
    vec3 noisePos = vec3(vertexAttribute.fragPos.xy * 10.0f, currentFrame * 0.01f);

    float persistence = 0.5f;
    float lacunarity = 2.0f;
    int octaves = 2;
    float noise = fbm(noisePos, octaves, persistence, lacunarity);
    // noise = fbm(noisePos + noise, octaves, persistence, lacunarity);
    // noise = fbm(noisePos + noise, octaves, persistence, lacunarity);
    // NOTE: FBM seem to generate value ranging from [-1, 1] so remap it to [0,1]
    noise = (1.0 + noise) / 2.0f;

    vec3 color = vec3(noise);
    FragColor = vec4(color, 1.0f);
}
