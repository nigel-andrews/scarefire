#version 450

layout( vertices = 4 ) out;

in vec3 vNormal[];
out vec3 tcsNormal[];

const uint DETAIL = 128;

void main() {
  gl_TessLevelOuter[0] = DETAIL;
  gl_TessLevelOuter[1] = DETAIL;
  gl_TessLevelOuter[2] = DETAIL;
  gl_TessLevelOuter[3] = DETAIL;

  gl_TessLevelInner[0] = DETAIL;
  gl_TessLevelInner[1] = DETAIL;

  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

  tcsNormal[gl_InvocationID] = vNormal[gl_InvocationID];
}
