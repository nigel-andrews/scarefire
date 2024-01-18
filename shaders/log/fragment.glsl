#version 450

// in vec3 vColor;

layout(location = 0) out vec4 color;

// uniform vec3 light_pos;

vec3 object_color = vec3(1.0, 1.0, 1.0);

void main() {
  color = vec4(object_color, 1.);
}
