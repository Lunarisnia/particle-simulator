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
// UE4's PseudoRandom function
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Shaders/Private/Random.ush
float pseudo(vec2 v) {
    v = fract(v / 128.) * 128. + vec2(-64.340622, -72.465622);
    return fract(dot(v.xyx * v.xyy, vec3(20.390625, 60.703125, 2.4281209)));
}

// Takes our xz positions and turns them into a random number between 0 and 1 using the above pseudo random function
float HashPosition(vec2 pos) {
    float _Seed = 0.12312348f;
    return pseudo(pos * vec2(_Seed, _Seed + 4));
}

// Generates a random gradient vector for the perlin noise lattice points, watch my perlin noise video for a more in depth explanation
vec2 RandVector(float seed) {
    float _GradientRotation = 0.0f;
    float theta = seed * 360 * 2 - 360;
    theta += _GradientRotation;
    theta = theta * PI / 180.0;
    return normalize(vec2(cos(theta), sin(theta)));
}

// Normal smoothstep is cubic -- to avoid discontinuities in the gradient, we use a quintic interpolation instead as explained in my perlin noise video
vec2 quinticInterpolation(vec2 t) {
    return t * t * t * (t * (t * vec2(6) - vec2(15)) + vec2(10));
}

// Derivative of above function
vec2 quinticDerivative(vec2 t) {
    return vec2(30) * t * t * (t * (t - vec2(2)) + vec2(1));
}

// it's perlin noise that returns the noise in the x component and the derivatives in the yz components as explained in my perlin noise video
vec3 perlin_noise2D(vec2 pos) {
    vec2 latticeMin = floor(pos);
    vec2 latticeMax = ceil(pos);

    vec2 remainder = fract(pos);

    // Lattice Corners
    vec2 c00 = latticeMin;
    vec2 c10 = vec2(latticeMax.x, latticeMin.y);
    vec2 c01 = vec2(latticeMin.x, latticeMax.y);
    vec2 c11 = latticeMax;

    // Gradient Vectors assigned to each corner
    vec2 g00 = RandVector(HashPosition(c00));
    vec2 g10 = RandVector(HashPosition(c10));
    vec2 g01 = RandVector(HashPosition(c01));
    vec2 g11 = RandVector(HashPosition(c11));

    // Directions to position from lattice corners
    vec2 p0 = remainder;
    vec2 p1 = p0 - vec2(1.0);

    vec2 p00 = p0;
    vec2 p10 = vec2(p1.x, p0.y);
    vec2 p01 = vec2(p0.x, p1.y);
    vec2 p11 = p1;

    vec2 u = quinticInterpolation(remainder);
    vec2 du = quinticDerivative(remainder);

    float a = dot(g00, p00);
    float b = dot(g10, p10);
    float c = dot(g01, p01);
    float d = dot(g11, p11);

    // Expanded interpolation freaks of nature from https://iquilezles.org/articles/gradientnoise/
    float noise = a + u.x * (b - a) + u.y * (c - a) + u.x * u.y * (a - b - c + d);

    vec2 gradient = g00 + u.x * (g10 - g00) + u.y * (g01 - g00) + u.x * u.y * (g00 - g10 - g01 + g11) + du * (u.yx * (a - b - c + d) + vec2(b, c) - a);
    return vec3(noise, gradient);
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

float perlinNoise(vec3 uv) {
    vec3 i = floor(uv);
    vec3 f = fract(uv);

    // Fade curve (smooth interpolation)
    // vec3 u = f * f * (3.0 - 2.0 * f);
    vec3 u = smoothstep(0.0f, 1.0f, f);

    // --- Corners on z = 0 plane ---
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
    return mix(mixY0, mixY1, u.z);
}

float fbm(vec3 position, int n, float persistence, float lacunarity) {
    float frequency = 1.0f;
    float amplitude = 0.5f;
    float total = 0.0f;
    float normalization = 0.0f;
    for (int i = 0; i < n; i++) {
        float noiseValue = perlin_noise2D(position.xy * frequency).x;
        total += noiseValue * 0.5f;
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
    vec3 noise_pos = vertexAttribute.fragPos / 0.5f;

    vec3 color = vec3(fbm(noise_pos, 6, 0.5f, 2.0f));
    FragColor = vec4(color, 1.0f);
}
