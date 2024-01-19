#include "particle_generator.hh"

#include <cstdlib>
#include <vector>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "shader.hh"
#include "state.hh"
#include "utils.hh"

FireGenerator::FireGenerator(ParticleGeneratorConfig config,
                             ShaderConfig particle_shader,
                             std::string compute_shader)
    : ParticleGenerator(config, particle_shader)
    , update_shader_id(
          assert_not_neg1(init_compute_shader(compute_shader),
                          "Invalid fire generator update compute shader"))
{
    glm::translate(this->config_.transform, glm::vec3(1, 0, 0));

    // Allocate VAO
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Allocate buffer for particles and buffer for mesh
    GLuint vbo_ids[2];
    glGenBuffers(2, vbo_ids);

    GLuint ssbo_id = vbo_ids[0];
    this->particle_buffer_id = ssbo_id;

    // Init particle buffer
    std::vector<Particle> particles;
    particles.reserve(config.max_particles);
    for (size_t i = 0; i < particles.capacity(); i++)
    {
        GLfloat position[3] = { (float)((rand() % 100) - 50) / 100.f,
                                (float)((rand() % 100) - 50) / 100.f,
                                (float)((rand() % 100) - 50) / 100.f };

        GLfloat velocity[3] = {
            (float)((rand() % 100) - 50) / 100.f,
            1.f + (float)((rand() % 100) - 50) / 100.f,
            (float)((rand() % 100) - 50) / 100.f,
        };

        particles.emplace_back(position, velocity, this->config_.particle_life);
    }
    // Bind VBO to VAO
    glBindBuffer(GL_ARRAY_BUFFER, ssbo_id);
    // Init VBO
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle),
                 particles.data(), GL_DYNAMIC_DRAW);

    // // Hardcoded attrib positions
    // // Particle position
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
    // nullptr);
    // // --skip: __padding_1 (float)
    // // Particle velocity
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),
    //                       reinterpret_cast<void*>(4 * sizeof(float)));
    // // Particle life
    // glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle),
    //                       reinterpret_cast<void*>(7 * sizeof(float)));
    //
    // // Enable attributes
    // glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(2);

    // Bind VBO as SSBO as well
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_id);

    ///////// Particle surface
    // clang-format off
    float P = 10.f;
    float particle_surf[] = {
        0.0f, P, 0.0f, P,
        P, 0.0f, P, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, P, 0.0f, P,
        P, P, P, P,
        P, 0.0f, P, 0.0f
    };
    // clang-format on

    // Allocate particle surface VBO
    GLuint surf_vbo = vbo_ids[1];
    this->particle_surface_vbo = surf_vbo;
    // Fill mesh buffer
    DOGL(glBindBuffer(GL_ARRAY_BUFFER, surf_vbo));
    DOGL(glBufferData(GL_ARRAY_BUFFER, sizeof(particle_surf), particle_surf,
                      GL_STATIC_DRAW));
    // Set mesh attributes
    DOGL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    DOGL(glEnableVertexAttribArray(0));

    // Reset VAO bind
    glBindVertexArray(0);
}

void FireGenerator::update(float anim_time)
{
    DOGL(glUseProgram(this->update_shader_id));

    SET_UNIFORM(
        this->update_shader_id, "emitter_pos",
        glUniform3fv(uniform_id, 1, (const float*)&this->config_.transform));

    SET_UNIFORM(this->update_shader_id, "starting_life",
                glUniform1f(uniform_id, this->config_.particle_life));

    SET_UNIFORM(this->update_shader_id, "anim_time",
                glUniform1f(uniform_id, anim_time));

    SET_UNIFORM(this->update_shader_id, "delta_time",
                glUniform1f(uniform_id, anim_time - this->last_update_time));

    DOGL(glDispatchCompute(this->config_.max_particles / 1024, 1, 1));

    this->last_update_time = anim_time;
}

void FireGenerator::render(const ProgramState& state) const
{
    glBlendFunc(this->config_.blending[0], this->config_.blending[1]);

    DOGL(glUseProgram(this->particle_shader_id));

    SET_UNIFORM(
        this->particle_shader_id, "view_proj",
        glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
                           (const float*)&state.scene.camera._view_proj));

    SET_UNIFORM(this->particle_shader_id, "emitter_transform",
                glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
                                   (const float*)&this->config_.transform));

    SET_UNIFORM(this->particle_shader_id, "max_life",
                glUniform1f(uniform_id, this->config_.particle_life));

    std::vector<Particle> particles;
    particles.reserve(this->config_.max_particles);
    glGetNamedBufferSubData(this->particle_buffer_id, 0,
                            this->config_.max_particles * sizeof(Particle),
                            particles.data());

    DOGL(glBindVertexArray(this->vao));
    DOGL(glBindBuffer(GL_ARRAY_BUFFER, this->particle_surface_vbo));

    for (const auto& particle : particles)
    {
        SET_UNIFORM(this->particle_shader_id, "particleLife",
                    glUniform1f(uniform_id, particle.life));
        SET_UNIFORM(this->particle_shader_id, "particlePos",
                    glUniformMatrix3fv(uniform_id, 1, GL_FALSE,
                                       (const float*)&particle.position));
        glDrawArrays(GL_TRIANGLES, 0, 2);
    }

    // Bind particle rect surface VBO + attrib
    // DOGL(glEnableVertexAttribArray(0));
    // DOGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

    // Bind particle data SSBO
    // DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0,
    //                       this->particle_buffer_id));

    // DOGL(glDrawElementsInstanced(GL_TRIANGLES,
    // int(this->config_.max_particles),
    //                              GL_UNSIGNED_INT, nullptr,
    //                              this->config_.max_particles));

    DOGL(glBindVertexArray(0));
}
