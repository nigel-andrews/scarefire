#version 450

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;

    gl_Position = gl_in[0].gl_Position + vec4(0, 0, 9, 1);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + vec4(0, 12, 49, 1);
    EmitVertex();

    EndPrimitive();
}
