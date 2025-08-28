#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform float currentTime;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
out VertexAttribute vertexAttribute;
out vec3 objectColor;

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

vec3 generateTerrain(vec3 position, vec3 normal, float noise) {
    vec3 pos = position;

    return pos + normal * noise;
}

void main()
{
    objectColor = aColor;
    vertexAttribute.textureCoord = aTexCoord;
    vertexAttribute.normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    // vertexAttribute.normal = aNormal;
    vertexAttribute.fragPos = vec3(model * vec4(aPos, 1.0f));
    vertexAttribute.tangent = aTangent;

    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0f)));
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0f)));
    vec3 B = cross(N, T);
    vertexAttribute.TBN = transpose(mat3(T, B, N));

    // vec3 displaced = aPos + aNormal;
    // displaced.x = (aPos + aNormal).x + sin((aPos + aNormal).y * 10.0f + currentTime) * 0.1f;
    // vec3 displaced = generateTerrain(aPos, fbm(aPos, 3));
    vec3 displaced = generateTerrain(aPos, aNormal, fbm(aPos, 16, 0.5f, 2.0f));
    gl_Position = projection * view * model * vec4(displaced, 1.0);
}
