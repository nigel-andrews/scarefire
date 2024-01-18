// clang-format off
#include <GL/glew.h>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "src/mesh.hh"
#include "src/state.hh"

static const std::vector<GLfloat> vertex_buffer_data {
  -0.5, 0.0, +0.5,
  +0.5, 0.0, +0.5,
  +0.5, 0.0, -0.5,
  -0.5, 0.0, -0.5,
};
// clang-format on

static struct ProgramState _state {};

void render() {
  // TODO: Implement render for log collection
  for (const auto &collection : _state.scene.collections)
    collection.render();
  // DOGL(glBindVertexArray(collection.vao_id));
  // DOGL(glDrawArrays(GL_PATCHES, 0, 4));
  // DOGL(glDrawArrays(GL_PATCHES, 0, 4));
}

void display() {
  _state.scene.model_view_matrix[4 * 3] = _state.offset[0];
  _state.scene.model_view_matrix[4 * 3 + 1] = _state.offset[1];
  _state.scene.model_view_matrix[4 * 3 + 2] = _state.offset[2];

  DOGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // use_shader(_state.log_program_id);
  render();

  DOGL(glBindVertexArray(0));

  glutSwapBuffers();
}

void anim() {
  _state.scene.anim_time += 0.1;
  glutPostRedisplay();
}

void timer(int) {
  anim();
  glutTimerFunc(33, timer, 0);
}

void init_anim() { glutTimerFunc(33, timer, 0); }

void mouse_button_handler(int button, int state, int x, int y) {
  (void)button;

  if (state == GLUT_DOWN) {
    _state.pos[0] = x;
    _state.pos[1] = y;
  } else {
    _state.held = false;
  }

  auto modifiers = glutGetModifiers();

  _state.shift = modifiers & GLUT_ACTIVE_SHIFT;
  _state.ctrl = modifiers & GLUT_ACTIVE_CTRL;
}

void mouse_motion_handler(int x, int y) {
  if (!_state.held) {
    _state.pos[0] = x;
    _state.pos[1] = y;
    _state.held = true;
  }

  if (_state.shift) {
    _state.offset[2] += (_state.pos[1] - y) / 100.;
  } else if (_state.ctrl) {
    _state.light_pos[0] -= (_state.pos[0] - x) / 1000.;
    _state.light_pos[1] += (_state.pos[1] - y) / 1000.;
  } else {
    _state.offset[0] -= (_state.pos[0] - x) / 1000.;
    _state.offset[1] += (_state.pos[1] - y) / 1000.;
  }

  _state.pos[0] = x;
  _state.pos[1] = y;

  // Now done in anim
  // glutPostRedisplay();
}

void window_resize(int width, int height) {
  DOGL(glViewport(0, 0, width, height));
}

void init_glut(int &argc, char *argv[]) {
  // glewExperimental = GL_TRUE;
  glutInit(&argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(1024, 1024);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Shader Programming");
  glutDisplayFunc(display);
  glutReshapeFunc(window_resize);
  glutMouseFunc(mouse_button_handler);
  glutMotionFunc(mouse_motion_handler);
}

bool init_glew() {
  if (glewInit()) {
    std::cerr << " Error while initializing glew";
    return false;
  }
  return true;
}

void init_GL() {
  DOGL(glEnable(GL_DEPTH_TEST));
  DOGL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  // DOGL(glEnable(GL_CULL_FACE));
  DOGL(glClearColor(0.4, 0.4, 0.4, 1.0));
  DOGL(glPatchParameteri(GL_PATCH_VERTICES, 4));
}

int main(int argc, char *argv[]) {
  init_glut(argc, argv);
  if (!init_glew())
    std::exit(-1);
  init_GL();
  init_anim();

  auto log_shader = ShaderConfig{
      .vertex = "shaders/log/vertex.shd",
      // .tesselation_control = "shaders/waves/tessellation_control.shd",
      // .tesselation_evaluation =
      // "shaders/waves/tessellation_evaluation.shd",
      .fragment = "shaders/log/fragment.shd",
  };

  MeshData mesh{};
  for (size_t i = 0; i < 10; i++) {
    for (size_t theta = 0; theta < 360; theta += 30) {
      // Generate cylinder vertices
    }
  }

  // _state.scene.collections

  glutMainLoop();
}
