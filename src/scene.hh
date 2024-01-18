#pragma once

#include <GL/glew.h>

#include "collection.hh"

struct Scene {
  // Object collections
  std::vector<struct Collection> collections;

  // Scene state
  float anim_time;

  // Lighting
  GLfloat light_pos[3] = {0.0, 1.0, 0.0};

  // Camera
  GLfloat model_view_matrix[16] = {
      0.577350, -0.3333, 0.57735, 0.00000, //
      0.000000, 0.66667, 0.57735, 0.00000, //
      -0.57735, -0.3333, 0.57735, 0.00000, //
      0.000000, 0.00000, -17.000, 1.00000, //
  };
  GLfloat projection_matrix[16] = {
      15.0000, 0.00000, 0.00000, 0.00000, //
      0.00000, 15.0000, 0.00000, 0.00000, //
      0.00000, 0.00000, -1.0002, -2.0000, //
      0.00000, 0.00000, -5.001,  0.00000, //
  };

  // Methods
  void render() const;
};
