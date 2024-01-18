#pragma once

#include "glm/glm.hpp"
#include "utils.hh"

class Camera
{
public:
    static glm::mat4 perspective(float fov_y, float ratio, float z_near);

    Camera();

    void set_view(const glm::mat4& matrix);
    void set_proj(const glm::mat4& matrix);

    void set_fov(float fov);
    void set_ratio(float ratio);

    glm::vec3 position() const;
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    const glm::mat4& projection_matrix() const;
    const glm::mat4& view_matrix() const;
    const glm::mat4& view_proj_matrix() const;

    bool is_orthographic() const;

    float fov() const;
    float ratio() const;

private:
    void update();

    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _view_proj;
};
