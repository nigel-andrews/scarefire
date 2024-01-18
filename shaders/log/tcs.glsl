#version 450

layout (vertices = 2) out;

void main(void)
{
    gl_TessLevelOuter[0] = 4.0;
    gl_TessLevelOuter[1] = 32.0;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
