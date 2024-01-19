#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 50) out;

uniform float log_depth;

in vec3 tesPosition[];

out vec3 gNormal;
out vec3 gPosition;

void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    gNormal = cross(p1 - p3, p2 - p3);

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();                           
        gl_Position = gl_in[i].gl_Position + vec4(gNormal, 1.);
        EmitVertex();                           
        for (float j = 0; j < 50 / 3; j++)
        {
            gl_Position = gl_in[i].gl_Position + vec4(vec3(gNormal) * j, 1.);
            EmitVertex();
        }
    }

    EndPrimitive();
}
