#include <GL/glew.h>

#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "bunny.hh"

#define TEST_OPENGL_ERROR()                                                    \
  do {                                                                         \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR)                                                    \
      std::cerr << "OpenGL ERROR! " << __LINE__ << std::endl;                  \
  } while (0)

GLuint bunny_vao_id;
GLuint program_id;
// bool m1 = false;
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

  GLuint mvm_id = glGetUniformLocation(program_id, "model_view_matrix");
  glUniformMatrix4fv(mvm_id, 1, GL_FALSE, model_view_matrix);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TEST_OPENGL_ERROR();
  glBindVertexArray(bunny_vao_id);
  TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
  TEST_OPENGL_ERROR();
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

  GLint vertex_location = glGetAttribLocation(program_id, "position");
  TEST_OPENGL_ERROR();
  GLint normal_smooth_location =
      glGetAttribLocation(program_id, "normalSmooth");
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

bool init_shaders() {
  std::string vertex_src = load("vertex.shd");
  std::string geometry_src = load("geometry.shd");
  std::string fragment_src = load("fragment.shd");
  GLuint shader_id[3];
  GLint compile_status = GL_TRUE;
  char *vertex_shd_src =
      (char *)std::malloc(vertex_src.length() * sizeof(char));
  char *geometry_shd_src =
      (char *)std::malloc(geometry_src.length() * sizeof(char));
  char *fragment_shd_src =
      (char *)std::malloc(fragment_src.length() * sizeof(char));
  vertex_src.copy(vertex_shd_src, vertex_src.length());
  geometry_src.copy(geometry_shd_src, geometry_src.length());
  fragment_src.copy(fragment_shd_src, fragment_src.length());

  shader_id[0] = glCreateShader(GL_VERTEX_SHADER);
  TEST_OPENGL_ERROR();
  shader_id[1] = glCreateShader(GL_GEOMETRY_SHADER);
  TEST_OPENGL_ERROR();
  shader_id[2] = glCreateShader(GL_FRAGMENT_SHADER);
  TEST_OPENGL_ERROR();

  glShaderSource(shader_id[0], 1, (const GLchar **)&(vertex_shd_src), 0);
  TEST_OPENGL_ERROR();
  glShaderSource(shader_id[1], 1, (const GLchar **)&(geometry_shd_src), 0);
  TEST_OPENGL_ERROR();
  glShaderSource(shader_id[2], 1, (const GLchar **)&(fragment_shd_src), 0);
  TEST_OPENGL_ERROR();

  for (int i = 0; i < 3; i++) {
    glCompileShader(shader_id[i]);
    TEST_OPENGL_ERROR();

    glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;
      glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char *)std::malloc(
          log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
      if (shader_log != 0) {
        glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
        std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
        std::free(shader_log);
      }
      std::free(vertex_shd_src);
      std::free(geometry_shd_src);
      std::free(fragment_shd_src);
      glDeleteShader(shader_id[0]);
      glDeleteShader(shader_id[1]);
      glDeleteShader(shader_id[2]);
      return false;
    }
  }
  std::free(vertex_shd_src);
  std::free(geometry_shd_src);
  std::free(fragment_shd_src);

  GLint link_status = GL_TRUE;
  program_id = glCreateProgram();
  TEST_OPENGL_ERROR();
  if (program_id == 0)
    return false;
  for (int i = 0; i < 3; i++) {
    glAttachShader(program_id, shader_id[i]);
    TEST_OPENGL_ERROR();
  }
  glLinkProgram(program_id);
  TEST_OPENGL_ERROR();
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    GLint log_size;
    char *program_log;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);
    program_log = (char *)std::malloc(
        log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
    if (program_log != 0) {
      glGetProgramInfoLog(program_id, log_size, &log_size, program_log);
      std::cerr << "Program " << program_log << std::endl;
      std::free(program_log);
    }
    glDeleteProgram(program_id);
    TEST_OPENGL_ERROR();
    glDeleteShader(shader_id[0]);
    TEST_OPENGL_ERROR();
    glDeleteShader(shader_id[1]);
    TEST_OPENGL_ERROR();
    glDeleteShader(shader_id[2]);
    TEST_OPENGL_ERROR();
    program_id = 0;
    return false;
  }
  glUseProgram(program_id);
  TEST_OPENGL_ERROR();
  return true;
}

int main(int argc, char *argv[]) {
  init_glut(argc, argv);
  if (!init_glew())
    std::exit(-1);
  init_GL();
  init_shaders();
  init_object_vbo();
  glutMainLoop();
}
