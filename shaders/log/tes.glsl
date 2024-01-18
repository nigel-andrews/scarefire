#version 450

layout(isolines, equal_spacing) in;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    gl_Position = projection_matrix * model_view_matrix * p1;
}
