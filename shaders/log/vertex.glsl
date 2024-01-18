#version 450

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
// uniform float anim_time;

in vec3 position;
// in vec3 normalSmooth;

out vec3 vColor;
// out vec3 vNormal;

// vec3 light_color = vec3(1.0, 1.0, 1.0);
// vec3 light_position = vec3(-10.0, -10.0, -50.0);

vec3 object_color = vec3(1.0, 1.0, 1.0);

void main() {
  gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);

  // float c = clamp(dot(normalize(position - light_position), normalSmooth), 0.0, 1.0);

  vColor = object_color; // * c;
                         //
  // vNormal = normalize(normalSmooth);
}
