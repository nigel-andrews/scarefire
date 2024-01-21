#pragma once

#include <GL/glew.h>
#include <optional>
#include <string>

#define SET_UNIFORM(Program, Name, Init)                                       \
    do                                                                         \
    {                                                                          \
        GLint uniform_id;                                                      \
        DOGL(uniform_id = glGetUniformLocation(Program, Name));                \
        if (uniform_id != -1)                                                  \
            DOGL(Init);                                                        \
    } while (0);

struct ShaderConfig
{
    std::string vertex;
    std::optional<std::string> tesselation_control = {};
    std::optional<std::string> tesselation_evaluation = {};
    std::optional<std::string> geometry = {};
    std::string fragment;
};

GLint init_arbitrary_shader(ShaderConfig config);
GLint init_compute_shader(std::string& path);
