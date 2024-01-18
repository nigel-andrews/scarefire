#version 450

layout(isolines, equal_spacing) in;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    float radius = 0.1;
    float u = gl_TessCoord.x;

    p1.yz = vec2(cos(u * 2.0 * 3.141592), sin(u * 2.0 * 3.141592)) * radius;

    gl_Position = projection_matrix * model_view_matrix * p1;
}