#include "camera.hh"

#include <GL/glu.h>
#include <cmath>

template <typename T>
inline constexpr T to_rad(T deg)
{
    return deg * T(0.01745329251994329576923690768489);
}

Mat<4> Camera::perspective(float fov_y, float ratio, float z_near)
{
    float f = 1.0f / std::tan(fov_y / 2.0f);
    return Mat<4>({ f / ratio, 0.0f, 0.0f, 0.0f, 0.0f, f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -1.0f, 0.0f, 0.0f, z_near, 0.0f });
}
Camera::Camera()
{
    _projection = perspective(to_rad(60.0f), 16.0f / 9.0f, 0.001f);
    DOGL(glMatrixMode(GL_MODELVIEW));
    glLoadIdentity();
    gluLookAt(2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glGetFloatv(GL_MODELVIEW, this->_view);
}
