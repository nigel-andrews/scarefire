#pragma once

#include <GL/glew.h>

#include "glm/fwd.hpp"
#include "scene.hh"

struct ProgramState
{
    // Input stuff
    float prev_input_time = 0.;
    bool held = false;
    bool shift = false;
    bool ctrl = false;
    glm::vec2 mouse_pos{ 0, 0 };
    glm::vec3 offset{ 0, 0, -5 };

    // Scene info
    struct Scene scene;
    glm::vec3 light_pos{ 0.0, 1.0, 0.0 };
    glm::vec3 look_at_target{ 0.0, 0.0, -1.0 };
    float log_depth = 1.;
};
