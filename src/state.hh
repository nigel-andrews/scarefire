#pragma once

#include <GL/glew.h>
#include <vector>

#include "collection.hh"
#include "scene.hh"

struct ProgramState {
  // OpenGL handles
  std::vector<struct Collection> collections;

  // Mouse stuff
  bool held = false;
  bool shift = false;
  bool ctrl = false;
  GLfloat offset[3] = {0, 0, -5};

  GLint pos[2] = {0, 0};

  GLfloat light_pos[3] = {0.0, 1.0, 0.0};

  // Scene info
  struct SceneData scene;
};
