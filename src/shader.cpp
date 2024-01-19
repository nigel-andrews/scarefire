#include "shader.hh"

// clang-format off
#include <GL/freeglut.h>
#include <GL/glu.h>
// clang-format on
#include <iostream>
#include <vector>

#include "utils.hh"

GLuint load_shader(std::string& path, int type)
{
    GLuint id;
    DOGL(id = glCreateShader(type));

    std::string src = load(path);
    const char* c_src = src.c_str();

    DOGL(glShaderSource(id, 1, &c_src, 0));

    return id;
}

bool checked_compile_shader(GLuint shader_ids[], GLuint id)
{
    GLint compile_status = GL_TRUE;

    DOGL(glCompileShader(shader_ids[id]));

    glGetShaderiv(shader_ids[id], GL_COMPILE_STATUS, &compile_status);

    // Error
    if (compile_status != GL_TRUE)
    {
        GLint log_size;
        char* shader_log;

        glGetShaderiv(shader_ids[id], GL_INFO_LOG_LENGTH, &log_size);
        shader_log = (char*)std::malloc(log_size + 1);
        if (shader_log)
        {
            glGetShaderInfoLog(shader_ids[id], log_size, &log_size, shader_log);
            std::cerr << "SHADER " << id << ": " << shader_log << std::endl;
            std::free(shader_log);
        }

        for (size_t j = 0; j <= id; j++)
            glDeleteShader(shader_ids[j]);
    }

    return compile_status == GL_TRUE;
}

bool checked_link_program(GLuint program_id, std::vector<GLuint>& shader_ids)
{
    DOGL(glLinkProgram(program_id));

    GLint link_status = GL_TRUE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

    // Error
    if (link_status != GL_TRUE)
    {
        GLint log_size;
        char* program_log;

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size + 1);
        if (program_log)
        {
            glGetProgramInfoLog(program_id, log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }

        DOGL(glDeleteProgram(program_id));
        for (size_t i = 0; i < shader_ids.size(); i++)
            DOGL(glDeleteShader(shader_ids[i]));
    }

    return link_status == GL_TRUE;
}

// TODO: Make generic with uniform init func in Collections
//
// void use_shader(GLuint shader_id, struct ProgramState state) {
//   DOGL(glUseProgram(shader_id));
//
//   SET_UNIFORM(shader_id, "model_view_matrix",
//               glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
//                                  state.scene.model_view_matrix));
//
//   SET_UNIFORM(shader_id, "projection_matrix",
//               glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
//                                  state.scene.projection_matrix));
//
//   SET_UNIFORM(shader_id, "light_pos",
//               glUniform3fv(uniform_id, 1, state.light_pos));
//
//   SET_UNIFORM(shader_id, "anim_time",
//               glUniform1f(uniform_id, state.scene.anim_time));
// }
//
GLint init_arbitrary_shader(ShaderConfig config)
{
    GLint program_id;
    std::vector<GLuint> shader_ids;

    shader_ids.emplace_back(load_shader(config.vertex, GL_VERTEX_SHADER));

    if (config.geometry.has_value())
        shader_ids.emplace_back(
            load_shader(config.geometry.value(), GL_GEOMETRY_SHADER));

    if (config.tesselation_control.has_value())
        shader_ids.emplace_back(load_shader(config.tesselation_control.value(),
                                            GL_TESS_CONTROL_SHADER));

    if (config.tesselation_evaluation.has_value())
        shader_ids.emplace_back(load_shader(
            config.tesselation_evaluation.value(), GL_TESS_EVALUATION_SHADER));

    shader_ids.emplace_back(load_shader(config.fragment, GL_FRAGMENT_SHADER));

    for (size_t i = 0; i < shader_ids.size(); i++)
        if (!checked_compile_shader(shader_ids.data(), i))
            return -1;

    DOGL(program_id = glCreateProgram());
    if (!program_id)
        return -1;

    for (size_t i = 0; i < shader_ids.size(); i++)
        DOGL(glAttachShader(program_id, shader_ids[i]));

    if (!checked_link_program(program_id, shader_ids))
        return -1;

    // Check for load success here, easier to debug
    DOGL(glUseProgram(program_id));

    return program_id;
}

GLint init_compute_shader(std::string& path)
{
    GLint program_id;

    GLuint compute_shader_id = load_shader(path, GL_COMPUTE_SHADER);
    std::vector<GLuint> shader_ids;
    shader_ids.emplace_back(compute_shader_id);

    DOGL(program_id = glCreateProgram());
    if (!program_id)
        return -1;

    DOGL(glAttachShader(program_id, compute_shader_id));

    if (!checked_link_program(program_id, shader_ids))
        return -1;

    DOGL(glUseProgram(program_id));

    return program_id;
}
