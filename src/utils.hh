#pragma once

#include <GL/glew.h>
#include <string>

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

#define STR(s) #s
#define DBG(VAR)                                                               \
  std::cout << STR(VAR) "[" << __LINE__ << "]: " << VAR << std::endl;

using Mat4 = GLfloat[16];

std::string load(const std::string &filename);
GLint assert_not_neg1(GLint value, std::string message);
