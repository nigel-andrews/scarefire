#version 450

layout (vertices = 2) out;

void main(void)
{
    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = 2;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
