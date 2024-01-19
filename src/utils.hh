#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>

using std::cout;

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR! " << __LINE__ << ": " << err           \
                      << std::endl;                                            \
    } while (0)

#define DOGL(...)                                                              \
    do                                                                         \
    {                                                                          \
        __VA_ARGS__;                                                           \
        TEST_OPENGL_ERROR();                                                   \
    } while (0)

#define STR(s) #s
#define DBG(VAR)                                                               \
    std::cout << STR(VAR) "[" << __LINE__ << "]: " << VAR << std::endl;

// using Mat4 = GLfloat[16];

std::string load(const std::string& filename);
GLint assert_not_neg1(GLint value, std::string message);
//
// template <int N>
// class Mat
// {
// public:
//     Mat() = default;
//
//     Mat(const std::initializer_list<GLfloat>& values)
//     {
//         int i = 0;
//         for (const auto value : values)
//         {
//             assert(i <= N * N);
//             this->data_[i++] = value;
//         }
//     }
//
//     Mat(const Mat& rhs)
//     {
//         for (auto i = 0; i < N * N; i++)
//             this->data_[i] = rhs.data_[i];
//     }
//
//     Mat& operator=(const Mat& rhs)
//     {
//         for (auto i = 0; i < N * N; i++)
//             this->data_[i] = rhs.data_[i];
//
//         return *this;
//     }
//
//     GLfloat& operator()(int i, int j)
//     {
//         assert(j * N + i < N * N);
//         return this->data_[j * N + i];
//     }
//
//     const GLfloat& operator()(int i, int j) const
//     {
//         assert(j * N + i < N * N);
//         return this->data_[j * N + i];
//     }
//
//     operator const GLfloat*() const
//     {
//         return this->data_;
//     }
//
//     operator GLfloat*()
//     {
//         return this->data_;
//     }
//
// private:
//     GLfloat data_[N * N];
// };
