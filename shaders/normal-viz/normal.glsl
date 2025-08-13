#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_Out {
    vec3 fragPos;
    vec3 normal;
} vs_in[];

out GS_Out {
    vec3 fragPos;
    vec3 normal;
} gs_out;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    gs_out.normal = vs_in[index].normal;
    gs_out.fragPos = vs_in[index].fragPos;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position +
                vec4(vs_in[index].normal, 0.0) * 0.2f);
    gs_out.normal = vs_in[index].normal;
    gs_out.fragPos = vs_in[index].fragPos;
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
