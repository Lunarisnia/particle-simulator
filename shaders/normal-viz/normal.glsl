#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_Out {
    vec3 normal;
} vs_in[];

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position +
                vec4(vs_in[index].normal, 0.0) * 0.04f);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}
