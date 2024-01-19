#version 450

layout(local_size_x = 1024) in;

layout(location = 0) uniform vec3 emitter_pos;
layout(location = 1) uniform float starting_life;
layout(location = 2) uniform float anim_time;
layout(location = 3) uniform float delta_time;

struct Particle {
    vec3 pos;
    float _padding_1;

    vec3 velocity;
    float life;
};

layout(std430, binding = 0) buffer particle_buffer {
    Particle particles[];
};

void reset_particle(inout Particle p) {
    p.pos = emitter_pos;
    p.life = starting_life;
}

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index != 0)
        index++;

    if (particles[index].life <= 0.) {
        reset_particle(particles[index]);
    }

    particles[index].pos += particles[index].velocity + sin(anim_time * vec3(1., 0., 1.));
    particles[index].life -= delta_time;
}
