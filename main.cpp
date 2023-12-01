#include <GL/glew.h>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "bunny.hh"

#define TEST_OPENGL_ERROR()                                                    \
  do {                                                                         \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR)                                                    \
      std::cerr << "OpenGL ERROR! " << __LINE__ << ": " << err << std::endl;   \
  } while (0)

#define DOGL(...)                                                              \
  do {                                                                         \
    __VA_ARGS__;                                                               \
    TEST_OPENGL_ERROR();                                                       \
  } while (0)

GLuint bunny_vao_id;

GLuint hair_program_id;
GLuint surface_program_id;
GLuint compute_program_id;

bool held = false;
GLfloat offset[3] = {0, 0, -17};
GLint pos[2] = {0, 0};
bool shift = false;

GLfloat model_view_matrix[16] = {
    0.577350, -0.3333, 0.57735, 0.00000, //
    0.000000, 0.66667, 0.57735, 0.00000, //
    -0.57735, -0.3333, 0.57735, 0.00000, //
    0.000000, 0.00000, -17.000, 1.00000, //
};
GLfloat projection_matrix[16] = {
    15.0000, 0.00000, 0.00000, 0.00000, //
    0.00000, 15.0000, 0.00000, 0.00000, //
    0.00000, 0.00000, -1.0002, -1.0000, //
    0.00000, 0.00000, -5.001,  0.00000, //
};

void use_shader(GLuint shader_id) {
  glUseProgram(shader_id);
  TEST_OPENGL_ERROR();

  GLuint mvm_id = glGetUniformLocation(shader_id, "model_view_matrix");
  glUniformMatrix4fv(mvm_id, 1, GL_FALSE, model_view_matrix);
  TEST_OPENGL_ERROR();

  GLuint proj_id = glGetUniformLocation(shader_id, "projection_matrix");
  glUniformMatrix4fv(proj_id, 1, GL_FALSE, projection_matrix);
  TEST_OPENGL_ERROR();
}

void render() {
  glBindVertexArray(bunny_vao_id);
  TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
  TEST_OPENGL_ERROR();
}

void window_resize(int width, int height) {
  // std::cout << "glViewport(0,0,"<< width << "," << height <<
  // ");TEST_OPENGL_ERROR();" << std::endl;
  glViewport(0, 0, width, height);
  TEST_OPENGL_ERROR();
}

void display() {
  model_view_matrix[4 * 3] = offset[0];
  model_view_matrix[4 * 3 + 1] = offset[1];
  model_view_matrix[4 * 3 + 2] = offset[2];

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TEST_OPENGL_ERROR();

  use_shader(surface_program_id);
  render();
  use_shader(hair_program_id);
  render();

  DOGL(glUseProgram(compute_program_id));
  DOGL(glDispatchCompute(1, 1024, 1));

  glBindVertexArray(0);
  TEST_OPENGL_ERROR();

  glutSwapBuffers();
}

void mouse_button_handler(int button, int state, int x, int y) {
  (void)button;

  if (state == GLUT_DOWN) {
    pos[0] = x;
    pos[1] = y;
  } else {
    held = false;
  }

  shift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
}

void mouse_motion_handler(int x, int y) {
  if (!held) {
    pos[0] = x;
    pos[1] = y;
    held = true;
  }

  if (!shift) {
    offset[0] -= (pos[0] - x) / 1000.;
    offset[1] += (pos[1] - y) / 1000.;
  } else {
    offset[2] += (pos[1] - y) / 100.;
  }

  pos[0] = x;
  pos[1] = y;

  glutPostRedisplay();
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
  glEnable(GL_DEPTH_TEST);
  TEST_OPENGL_ERROR();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  TEST_OPENGL_ERROR();
  glEnable(GL_CULL_FACE);
  TEST_OPENGL_ERROR();
  glClearColor(0.4, 0.4, 0.4, 1.0);
  TEST_OPENGL_ERROR();
}

void init_object_vbo() {
  int max_nb_vbo = 5;
  int nb_vbo = 0;
  int index_vbo = 0;
  GLuint vbo_ids[max_nb_vbo];

  GLint vertex_location;
  DOGL(vertex_location = glGetAttribLocation(hair_program_id, "position"));
  GLint normal_smooth_location;
  DOGL(normal_smooth_location =
           glGetAttribLocation(hair_program_id, "normalSmooth"));

  DOGL(glGenVertexArrays(1, &bunny_vao_id));
  DOGL(glBindVertexArray(bunny_vao_id));

  if (vertex_location != -1)
    nb_vbo++;
  if (normal_smooth_location != -1)
    nb_vbo++;
  DOGL(glGenBuffers(nb_vbo, vbo_ids));

  if (vertex_location != -1) {
    DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]));
    DOGL(glBufferData(GL_ARRAY_BUFFER,
                      vertex_buffer_data.size() * sizeof(float),
                      vertex_buffer_data.data(), GL_STATIC_DRAW));
    DOGL(glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0));
    DOGL(glEnableVertexAttribArray(vertex_location));
    DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[index_vbo - 1]));
  }

  if (normal_smooth_location != -1) {
    DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]));
    DOGL(glBufferData(GL_ARRAY_BUFFER,
                      normal_smooth_buffer_data.size() * sizeof(float),
                      normal_smooth_buffer_data.data(), GL_STATIC_DRAW));
    DOGL(glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0,
                               0));
    DOGL(glEnableVertexAttribArray(normal_smooth_location));
    DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[index_vbo - 1]));
  }

  glBindVertexArray(0);
}

std::string load(const std::string &filename) {
  std::ifstream input_src_file(filename, std::ios::in);
  std::string ligne;
  std::string file_content = "";
  if (input_src_file.fail()) {
    std::cerr << "FAIL\n";
    return "";
  }
  while (getline(input_src_file, ligne)) {
    file_content = file_content + ligne + "\n";
  }
  file_content += '\0';
  input_src_file.close();
  return file_content;
}

struct ShaderConfig {
  std::string vertex;
  std::optional<std::string> tesselation_control = {};
  std::optional<std::string> tesselation_evaluation = {};
  std::optional<std::string> geometry = {};
  std::string fragment;
};

GLuint load_shader(std::string &path, int type) {
  GLuint id;
  DOGL(id = glCreateShader(type));

  std::string src = load(path);
  const char *c_src = src.c_str();

  DOGL(glShaderSource(id, 1, &c_src, 0));

  return id;
}

bool checked_compile_shader(GLuint shader_ids[], GLuint id) {
  GLint compile_status = GL_TRUE;

  DOGL(glCompileShader(shader_ids[id]));

  glGetShaderiv(shader_ids[id], GL_COMPILE_STATUS, &compile_status);

  // Error
  if (compile_status != GL_TRUE) {
    GLint log_size;
    char *shader_log;

    glGetShaderiv(shader_ids[id], GL_INFO_LOG_LENGTH, &log_size);
    shader_log = (char *)std::malloc(log_size + 1);
    if (shader_log) {
      glGetShaderInfoLog(shader_ids[id], log_size, &log_size, shader_log);
      std::cerr << "SHADER " << id << ": " << shader_log << std::endl;
      std::free(shader_log);
    }

    for (size_t j = 0; j <= id; j++)
      glDeleteShader(shader_ids[j]);
  }

  return compile_status == GL_TRUE;
}

bool checked_link_program(GLuint program_id, std::vector<GLuint> &shader_ids) {
  DOGL(glLinkProgram(program_id));

  GLint link_status = GL_TRUE;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

  // Error
  if (link_status != GL_TRUE) {
    GLint log_size;
    char *program_log;

    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);
    program_log = (char *)std::malloc(log_size + 1);
    if (!program_log) {
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

GLint init_arbitrary_shader(ShaderConfig config) {
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
    shader_ids.emplace_back(load_shader(config.tesselation_evaluation.value(),
                                        GL_TESS_EVALUATION_SHADER));

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

GLint init_compute_shader(std::string &&path) {
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

int main(int argc, char *argv[]) {
  init_glut(argc, argv);
  if (!init_glew())
    std::exit(-1);
  init_GL();

  // glPatchParameteri(GL_PATCH_VERTICES, 3);

  surface_program_id = init_arbitrary_shader(ShaderConfig{
      .vertex = "shaders/surface/vertex.shd",
      .fragment = "shaders/surface/fragment.shd",
  });

  hair_program_id = init_arbitrary_shader(ShaderConfig{
      .vertex = "shaders/hair/vertex.shd",
      // .tesselation_control = "shaders/hair/tesselation_control.shd",
      // .tesselation_evaluation = "shaders/hair/tesselation_evaluation.shd",
      .geometry = "shaders/hair/geometry.shd",
      .fragment = "shaders/hair/fragment.shd",
  });

  compute_program_id = init_compute_shader("shaders/compute.shd");

  init_object_vbo();

  glutMainLoop();
}
