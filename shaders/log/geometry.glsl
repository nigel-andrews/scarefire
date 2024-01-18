#version 450

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 tesPosition[];

out vec3 gNormal;
out vec3 gPosition;

void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;

    vec3 p3 = p1 + vec3(0., 0., 1.);
    vec3 p4 = p2 + vec3(0., 0., 1.);

    gNormal = -cross(p1 - p2, p1 - p3);

    gl_Position = gl_in[0].gl_Position;
    gPosition = tesPosition[0];
    EmitVertex();
    gl_Position = vec4(p3, 1.0);
    EmitVertex();
    gPosition = tesPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = vec4(p4, 1.0);
    EmitVertex();

    EndPrimitive();
}
