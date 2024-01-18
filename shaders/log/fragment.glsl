#version 450

in vec3 vColor;

layout(location = 0) out vec4 color;

uniform vec3 light_pos;

void main() {
  color = vec4(vColor, 1.);
}
