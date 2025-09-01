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

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight[2];

uniform vec2 mousePosition;
uniform float globalFloat;
uniform float globalFloat2;

uniform int octave;
uniform float persistence;
uniform float lacunarity;

const uint k = 1103515245U;
vec3 hash33(uvec3 x)
{
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;

    return vec3(x) * (1.0 / float(0xffffffffU));
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

// NOTE: the grid artifact came from the fact that I used integer hash and it seems to tile similarly to how fract works simply replacing the hash function works wonder
void main() {
    float eps = 0.001f;
    vec3 color = vec3(0.0f);

    vec3 pos = vertexAttribute.rawPos + vec3(globalFloat);

    int octaves = octave;
    float d0 = fbm(pos, octaves, persistence, lacunarity);
    float dx = fbm(pos + vec3(eps, 0.0f, 0.0f), octaves, persistence, lacunarity);
    float dy = fbm(pos + vec3(0.0f, eps, 0.0f), octaves, persistence, lacunarity);
    float dz = fbm(pos + vec3(0.0f, 0.0f, eps), octaves, persistence, lacunarity);

    vec3 dNormal = vec3(
            (dx - d0) / eps,
            (dy - d0) / eps,
            (dz - d0) / eps
        );
    dNormal = normalize(vertexAttribute.normal - dNormal);

    PointLight pLight = pointLight[0];
    vec3 lightDir = normalize(pLight.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(dNormal, lightDir));
    vec3 diffuse = vec3(1.0f) * diff;
    color += diffuse;

    // color = vec3(vertexAttribute.textureCoord, 0.0f);
    // float f = fbm(pos * 20.0f, 8, 0.5f, 2.0f);
    // color = vec3(f * 20.0f);
    FragColor = vec4(color, 1.0f);
}
