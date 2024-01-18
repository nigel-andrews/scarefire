#pragma once

#include <GL/glew.h>

#include "scene.hh"

struct ProgramState
{
    // Mouse stuff
    bool held = false;
    bool shift = false;
    bool ctrl = false;
    GLfloat offset[3] = { 0, 0, -5 };

    GLint pos[2] = { 0, 0 };

    GLfloat light_pos[3] = { 0.0, 1.0, 0.0 };

    // Scene info
    struct Scene scene;
    GLfloat look_at_target[3] = { 0.0, 0.0, -1.0 };
};
