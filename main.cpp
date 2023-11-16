#include <GL/glew.h>

#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "bunny.hh"

#define TEST_OPENGL_ERROR()                                                    \
  do {                                                                         \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR)                                                    \
      std::cerr << "OpenGL ERROR! " << __LINE__ << std::endl;                  \
  } while (0)

#define DOGL(...)                                                              \
  {                                                                            \
    __VA_ARGS__;                                                               \
    TEST_OPENGL_ERROR();                                                       \
  }

GLuint bunny_vao_id;

GLuint hair_program_id;
GLuint surface_program_id;

bool held = false;
GLfloat offset[2] = {0, 0};
GLint pos[2] = {0, 0};

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
    0.00000, 0.00000, -10.001, 0.00000, //
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
  // GLuint offset_id = glGetUniformLocation(program_id, "pan");
  // glUniform2fv(offset_id, 1, offset);
  model_view_matrix[12] = offset[0];
  model_view_matrix[13] = offset[1];

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TEST_OPENGL_ERROR();

  use_shader(surface_program_id);
  render();
  use_shader(hair_program_id);
  render();

  glBindVertexArray(0);
  TEST_OPENGL_ERROR();

  glutSwapBuffers();
}

void mouse_button_handler(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    pos[0] = x;
    pos[1] = y;
  } else {
    pos[0] = 0;
    pos[1] = 0;
    held = false;
  }
}

void mouse_motion_handler(int x, int y) {
  if (!held) {
    pos[0] = x;
    pos[1] = y;
    held = true;
  }

  offset[0] -= (pos[0] - x) / 1000.;
  offset[1] += (pos[1] - y) / 1000.;

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

  GLint vertex_location = glGetAttribLocation(hair_program_id, "position");
  TEST_OPENGL_ERROR();
  GLint normal_smooth_location =
      glGetAttribLocation(hair_program_id, "normalSmooth");
  TEST_OPENGL_ERROR();

  glGenVertexArrays(1, &bunny_vao_id);
  TEST_OPENGL_ERROR();
  glBindVertexArray(bunny_vao_id);
  TEST_OPENGL_ERROR();

  if (vertex_location != -1)
    nb_vbo++;
  if (normal_smooth_location != -1)
    nb_vbo++;
  glGenBuffers(nb_vbo, vbo_ids);
  TEST_OPENGL_ERROR();

  if (vertex_location != -1) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                 vertex_buffer_data.data(), GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_location);
    TEST_OPENGL_ERROR();
  }

  if (normal_smooth_location != -1) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER,
                 normal_smooth_buffer_data.size() * sizeof(float),
                 normal_smooth_buffer_data.data(), GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(normal_smooth_location);
    TEST_OPENGL_ERROR();
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
  std::optional<std::string> geometry;
  std::string fragment;
};

GLuint load_shader(std::string &path, int type) {
  GLuint id;
  DOGL(id = glCreateShader(type));

  std::string src = load(path);
  const char *c_src = src.c_str();
  // char *shd_src = (char *)std::malloc(src.length() * sizeof(char));
  // src.copy(shd_src, src.length());

  DOGL(glShaderSource(id, 1, &c_src, 0));

  // std::free(shd_src);

  return id;
}

GLint init_arbitrary_shader(ShaderConfig config) {
  GLint program_id;
  std::vector<GLuint> shader_ids;

  shader_ids.emplace_back(load_shader(config.vertex, GL_VERTEX_SHADER));

  if (config.geometry.has_value())
    shader_ids.emplace_back(
        load_shader(config.geometry.value(), GL_GEOMETRY_SHADER));

  shader_ids.emplace_back(load_shader(config.fragment, GL_FRAGMENT_SHADER));

  GLint compile_status = GL_TRUE;

  for (int i = 0; i < shader_ids.size(); i++) {
    DOGL(glCompileShader(shader_ids[i]));

    glGetShaderiv(shader_ids[i], GL_COMPILE_STATUS, &compile_status);

    // Error
    if (compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;

      glGetShaderiv(shader_ids[i], GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char *)std::malloc(log_size + 1);
      if (shader_log) {
        glGetShaderInfoLog(shader_ids[i], log_size, &log_size, shader_log);
        std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
        std::free(shader_log);
      }

      for (int j = 0; j <= i; j++)
        glDeleteShader(shader_ids[j]);

      return -1;
    }
  }

  DOGL(program_id = glCreateProgram());
  if (!program_id)
    return -1;

  for (int i = 0; i < shader_ids.size(); i++)
    DOGL(glAttachShader(program_id, shader_ids[i]));

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
    for (int i = 0; i < shader_ids.size(); i++)
      DOGL(glDeleteShader(shader_ids[i]));

    return -1;
  }

  // Check for load success here, easier to debug
  DOGL(glUseProgram(program_id));

  return program_id;
}

int main(int argc, char *argv[]) {
  init_glut(argc, argv);
  if (!init_glew())
    std::exit(-1);
  init_GL();

  surface_program_id = init_arbitrary_shader(ShaderConfig{
      .vertex = "shaders/surface/vertex.shd",
      .geometry = {},
      .fragment = "shaders/surface/fragment.shd",
  });

  hair_program_id = init_arbitrary_shader(ShaderConfig{
      .vertex = "shaders/hair/vertex.shd",
      .geometry = "shaders/hair/geometry.shd",
      .fragment = "shaders/hair/fragment.shd",
  });

  init_object_vbo();
  glutMainLoop();
}
