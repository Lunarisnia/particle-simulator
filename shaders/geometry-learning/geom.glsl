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

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a));
}

vec4 explode(vec4 position, vec3 normal) {
    return position + vec4(normal * (currentTime * 0.25f), 0.0f);
}

void main() {
    vec3 normal = GetNormal();
    gl_Position = gl_in[0].gl_Position;
    gs_vertexAttribute = vertexAttribute[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gs_vertexAttribute = vertexAttribute[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gs_vertexAttribute = vertexAttribute[2];
    EmitVertex();
    EndPrimitive();
}
