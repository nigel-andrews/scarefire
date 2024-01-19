#version 450

#define PI 3.141592

layout(isolines, equal_spacing, ccw) in;

out vec3 tesPosition;

void main(void)
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    float radius = 0.3;
    float u = gl_TessCoord.x;

    p1.yz = vec2(cos(u * 2.0 * PI), sin(u * 2.0 * PI)) * radius;

    gl_Position = p1;
    tesPosition = p1.xyz;
}
