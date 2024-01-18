#pragma once

#include "utils.hh"

class Camera
{
public:
    static Mat<4> perspective(float fov_y, float ratio, float z_near);

    Camera();

    void set_view(const Mat<4>& matrix);
    void set_proj(const Mat<4>& matrix);

    Mat<4> _projection;
    Mat<4> _view;
};
