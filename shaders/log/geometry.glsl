#version 450

layout(lines) in;
layout(triangle_strip, max_vertices = 3) out;

void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;

    vec3 p3 = mix(p1, p2, 0.5) + vec3(1);

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = vec4(p3, 1.0);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();
}
