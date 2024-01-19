#pragma once

#include <GL/glew.h>

#include "glm/fwd.hpp"
#include "scene.hh"

struct ProgramState
{
    // Mouse stuff
    bool held = false;
    bool shift = false;
    bool ctrl = false;
    glm::vec2 mouse_pos{ 0, 0 };

    // Timing
    float anim_time = 0;
    float prev_input_time = 0;

    glm::vec3 light_pos{ 0.0, 1.0, 0.0 };

    // Scene info
    struct Scene scene;
    glm::vec3 look_at_target{ 0.0, 0.0, -1.0 };
};
