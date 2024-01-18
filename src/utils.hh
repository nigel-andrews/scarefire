#pragma once

#include <GL/glew.h>
#include <assert.h>
#include <exception>
#include <initializer_list>
#include <string>

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

template <int N>
class Mat
{
public:
    Mat()
        : data_(new GLfloat[N * N])
    {}

    Mat(const std::initializer_list<GLfloat>& values)
    {
        int i = 0;
        for (const auto value : values)
        {
            assert(i <= N * N);
            this->data_[i++] = value;
        }
    }

    Mat(const Mat& rhs)
    {
        for (auto i = 0; i < N * N; i++)
        {
            this->data_[i] = rhs.data_[i];
        }
    }

    ~Mat()
    {
        delete[] this->data_;
    }

    GLfloat& operator()(int i, int j)
    {
        assert(j * N + i < N * N);
        return this->data_[j * N + i];
    }

    const GLfloat& operator()(int i, int j) const
    {
        return this->data_[j * N + i];
    }

private:
    GLfloat data_[N * N];
};
