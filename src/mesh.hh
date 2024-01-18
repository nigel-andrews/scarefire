#pragma once

#include <GL/glew.h>
#include <optional>
#include <vector>

struct MeshData {
  std::vector<GLfloat> vertices;
  std::optional<std::vector<GLfloat>> normals;
};
