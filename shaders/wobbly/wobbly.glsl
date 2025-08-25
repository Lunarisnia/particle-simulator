#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float currentTime;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
    vec3 tangent;

    mat3 TBN;
};
in VertexAttribute vertexAttribute[];
out VertexAttribute gs_vertexAttribute;

const uint k = 1103515245U;
vec3 hash33(uvec3 x)
{
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;
    x = ((x >> 8U) ^ x.yzx) * k;

    return vec3(x) * (1.0 / float(0xffffffffU));
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a));
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

vec4 explode(vec4 position, vec3 normal) {
    return position + vec4(normal * (currentTime * 0.25f), 0.0f);
}

vec4 wobble(vec4 position, vec3 normal, float time) {
    vec4 pos = position;
    pos.x = position.x + sin(position.y * 10.0f + time) * 0.1f;
    return pos;
}

void main() {
    vec3 normal = GetNormal();
    // gl_Position = gl_in[0].gl_Position;
    gl_Position = wobble(gl_in[0].gl_Position, vertexAttribute[0].normal, currentTime);
    gs_vertexAttribute = vertexAttribute[0];
    EmitVertex();
    gl_Position = wobble(gl_in[1].gl_Position, vertexAttribute[1].normal, currentTime);
    gs_vertexAttribute = vertexAttribute[1];
    EmitVertex();
    gl_Position = wobble(gl_in[2].gl_Position, vertexAttribute[2].normal, currentTime);
    gs_vertexAttribute = vertexAttribute[2];
    EmitVertex();
    EndPrimitive();
}
