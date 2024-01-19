#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

struct MeshData
{
    std::vector<glm::vec3> vertices;
    std::optional<std::vector<glm::vec3>> normals;
};
