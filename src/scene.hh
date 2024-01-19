#pragma once

#include <GL/glew.h>

#include "camera.hh"
#include "collection.hh"
#include "particle_generator.hh"

struct Scene
{
    // Object collections
    std::vector<struct Collection> collections;
    std::optional<FireGenerator> fire;

    // Scene state
    float anim_time;

    // Lighting
    GLfloat light_pos[3] = { 0.0, 1.0, 0.0 };

    Camera camera;

    // Methods
    void render() const;
};
