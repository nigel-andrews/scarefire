#version 450

// layout(location = 0) in vec3 position;
// layout(location = 1) in vec3 velocity;
// layout(location = 2) in float life;

// struct Particle {
//     vec3 pos;
//     float _padding_1;
//
//     vec3 velocity;
//     float life;
// };

// layout(std430, binding = 0) buffer particle_buffer {
//     Particle particles[];
// };


layout(location = 0) in vec3 meshVertPos;

uniform mat4 view_proj;
uniform mat4 emitter_transform;
uniform vec3 particlePos;

void main() {
    gl_Position = view_proj * emitter_transform * vec4(particlePos, 1.);
}
