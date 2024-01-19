#version 450

out vec4 color;

uniform float max_life;
uniform float particleLife;

void main() {
    color = vec4(mix(vec3(1., 1., 0.), vec3(1., 0., 0.), particleLife / max_life), 1.);
}
