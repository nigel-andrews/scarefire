#version 450

// in vec3 vColor;
in vec3 gNormal;
in vec3 gPosition;
in vec3 gColor;

layout(location = 0) out vec4 color;

uniform vec3 light_pos;

vec3 object_color = vec3(1.0, 1.0, 1.0);

void main() {
  vec3 light_dir = normalize(light_pos - gPosition);
  color = vec4(clamp(object_color * dot(light_dir, gNormal), 0.0, 1.0), 1.0);
  // color = vec4(gl_FragCoord.z);
  // color = vec4(gColor, 1.);
}
