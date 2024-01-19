#version 450

in vec3 position;

vec3 object_color = vec3(1.0, 1.0, 1.0);

void main() {
  gl_Position = vec4(position, 1.);
}
