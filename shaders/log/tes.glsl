#version 450

#define PI 3.141592

layout(quads, equal_spacing, ccw) in;

void main(void)
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 center = mix(p1, p2, gl_TessCoord.y);

    gl_Position = center;
}
