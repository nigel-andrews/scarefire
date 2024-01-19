#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <string>

#include "shader.hh"
#include "utils.hh"

struct ProgramState;

struct ParticleGeneratorConfig
{
    GLenum blending[2] = { GL_SRC_ALPHA, GL_ONE };

    size_t max_particles = 1000;
    float particle_life = 100000;
    float particle_size = 0.1;

    glm::mat4 transform{ 1.f };
};

struct ParticleGenerator
{
    ParticleGeneratorConfig config_;
    GLuint particle_shader_id;
    GLuint particle_surface_vbo;
    float last_update_time = 0;
    GLuint vao;

    ParticleGenerator(ParticleGeneratorConfig config,
                      ShaderConfig particle_shader)
        : config_(config)
        , particle_shader_id(
              assert_not_neg1(init_arbitrary_shader(particle_shader),
                              "Failed to initialize shader")){};

    virtual void update(float anim_time) = 0;
    virtual void render(const ProgramState& state) const = 0;
};

struct FireGenerator : ParticleGenerator
{
    GLuint update_shader_id;
    GLuint particle_buffer_id;

    FireGenerator(ParticleGeneratorConfig config, ShaderConfig particle_shader,
                  std::string compute_shader);

    void update(float anim_time);
    void render(const ProgramState& state) const;
};

struct Particle
{
    GLfloat position[3];
    float _padding_1;

    GLfloat velocity[3];
    float life;

    Particle(GLfloat position[3], GLfloat velocity[3], float life)
        : life(life)
    {
        for (size_t i = 0; i < 3; i++)
        {
            this->position[i] = position[i];
            this->velocity[i] = velocity[i];
        }
    }
};
