#version 330 core
layout(location = 0) out vec4 FragColor;

uniform float currentTime;
struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;
    vec3 rawPos;

    mat3 TBN;
};
in VertexAttribute vertexAttribute;
in vec3 objectColor;

struct Camera {
    vec3 position;
};
uniform Camera camera;

uniform vec2 mousePosition;

const uint k = 1103515245U;
vec3 hash33(uvec3 x)
{
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;

    return vec3(x) * (1.0 / float(0xffffffffU));
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
    // vec2 gradient = g00 + u.x * (g10 - g00) + u.y * (g01 - g00) + u.x * u.y * (g00 - g10 - g01 + g11) + du * (u.yx * (a - b - c + d) + vec2(b, c) - a);
    return mix(mixY0, mixY1, u.z);
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

// FIX: This produce weird square artifacts
void main() {
    vec3 color = vec3(0.0f);

    float displacement = fbm(vertexAttribute.fragPos, 16, 0.5f, 2.0f);

    vec3 dispPos = vertexAttribute.fragPos + vertexAttribute.normal * displacement;

    vec3 dpdx = dFdx(dispPos);
    vec3 dpdy = dFdy(dispPos);

    vec3 dNormal = normalize(cross(dpdx, dpdy));
    // vec3 dNormal = normalize(dispPos);

    vec3 lightDir = normalize(vec3(0.5f));
    float diff = dot(dNormal, lightDir);
    color = vec3(1.0f) * diff;

    FragColor = vec4(color, 1.0f);
}
