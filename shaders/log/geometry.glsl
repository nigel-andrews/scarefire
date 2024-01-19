#version 450

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 tesPosition[];

out vec3 gNormal;
out vec3 gPosition;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
mat4 view_proj = projection_matrix * model_view_matrix;


void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;

    vec3 p3 = p1 + vec3(1., 0., 0.);
    vec3 p4 = p2 + vec3(1., 0., 0.);

    gNormal = cross(p1 - p2, p1 - p3);

    gl_Position = view_proj * gl_in[0].gl_Position;
    gPosition = tesPosition[0];
    EmitVertex();
    gl_Position = view_proj * vec4(p3, 1.0);
    EmitVertex();
    gPosition = tesPosition[1];
    gl_Position = view_proj * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = view_proj * vec4(p4, 1.0);
    EmitVertex();

    EndPrimitive();
}
