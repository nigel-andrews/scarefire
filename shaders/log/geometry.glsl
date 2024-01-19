#version 450

#define VERTICES 50

layout(triangles) in;
layout(triangle_strip, max_vertices = VERTICES) out;

uniform float log_depth;

in vec3 tesPosition[];

out vec3 gNormal;
out vec3 gPosition;
out vec3 gColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void set_color(vec3 pos) {
    gColor = vec3(rand(pos.xy), rand(pos.xz), rand(pos.yz));
}

void main(void)
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    gNormal = normalize(cross(p1 - p3, p2 - p3));

    gl_Position = vec4(p1, 1.);
    set_color(p1);
    EmitVertex();

    // -2 to account for start/endcap center vertices
    for (int i = 0; i < VERTICES - 2; i += 2)
    {
        // Cross: (i / (VERTICES - 2)) X (? / log_depth)
        float factor = log_depth * float(i) / (VERTICES - 2);

        vec3 second = p2 + factor * gNormal;
        gl_Position = vec4(second, 1.);
        set_color(second);
        EmitVertex();

        vec3 third = p3 + factor * gNormal;
        gl_Position = vec4(third, 1.);
        set_color(third);
        EmitVertex();
    }

    vec3 last = p1 + gNormal * log_depth;
    gl_Position = vec4(last, 1.);
    set_color(last);
    EmitVertex();

    EndPrimitive();
}
