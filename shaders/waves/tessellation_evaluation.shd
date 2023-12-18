#version 450

layout( quads, equal_spacing, ccw ) in;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

uniform float anim_time;

in vec3 tcsNormal[];
out vec3 tesNormal;

out vec3 tesPosition;
out vec3 tesColor;

// float wave(float amplitude, float freq, int octave, float x, float y) {
//   return (amplitude / float(octave)) 
//     * sin(anim_time + (freq * float(octave)) * (gl_TessCoord.x * x + gl_TessCoord.y * y));
// }
//
// float wave_derivative(float amplitude, float freq, int octave, float x, float y) {
//   return (freq * float(octave)) * cos(anim_time + (freq * float(octave)) * (gl_TessCoord.x * x + gl_TessCoord.y * y));
// }

const vec2 X = vec2(1., 0.);
const vec2 Y = vec2(0., 1.);

struct Wave {
  float amplitude;
  float freq;
  vec2 dir;
} waves[] = {
  { 1. / 10., 10., X},
  { 1. / 10., 10., Y},
};

float wave(uint i, vec2 components) {
  return waves[i].amplitude * sin(
    anim_time + waves[i].freq * (components.x + components.y)
  );
}

float deriv(uint i, vec2 components) {
  return waves[i].amplitude * waves[i].freq * cos(
    anim_time + waves[i].freq * (components.x + components.y)
  );
}

void main() {
  float wave_result = 0.;
  float wave_deriv = 0.;
  for (uint i = 0; i < waves.length(); i++)
  {
    vec2 components = gl_TessCoord.xy * waves[i].dir;
    wave_result += wave(i, components);
    wave_deriv += deriv(i, components);
  }

  vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
  vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
  vec4 p = mix(p1, p2, gl_TessCoord.y);
  gl_Position = projection_matrix * model_view_matrix * (p + vec4(0., wave_result, 0., 0.));
  tesPosition = p.xyz;

  // vec3 n1 = mix(tcsNormal[0], tcsNormal[1], gl_TessCoord.x);
  // vec3 n2 = mix(tcsNormal[3], tcsNormal[2], gl_TessCoord.x);
  // vec3 n = mix(n1, n2, gl_TessCoord.y);
  vec3 lhs = vec3(1, wave_deriv, 0);
  vec3 rhs = vec3(0, wave_deriv, 1);
  vec3 n = -normalize(cross(lhs, rhs));
  tesNormal = vec3(n.x, n.y, n.z);
}
