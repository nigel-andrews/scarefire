#include "collection.hh"

#include <iostream>

#include "shader.hh"
#include "utils.hh"

Collection::Collection(MeshData mesh, struct ShaderConfig shader,
                       std::vector<glm::mat4>&& transforms)
    : mesh_(mesh)
    , shaders_(shader)
    , transforms(std::move(transforms))
{
    this->program_id = assert_not_neg1(init_arbitrary_shader(shader),
                                       "Failed to initialize shader");

    GLint vertex_location;
    DOGL(vertex_location = glGetAttribLocation(this->program_id, "position"));

    GLint normal_location;
    DOGL(normal_location =
             glGetAttribLocation(this->program_id, "normalSmooth"));
    bool init_normals = mesh.normals.has_value() && (normal_location != -1);

    if (mesh.normals.has_value() && (normal_location == -1))
    {
        std::cerr
            << "[C++][WARN] Normals provided but normal input not found in "
               "vertex shader."
            << std::endl;
    }

    // Init and bind VAO
    DOGL(glGenVertexArrays(1, &this->vao_id));
    DOGL(glBindVertexArray(this->vao_id));

    // Allocate VBOs
    GLuint vbo_ids[1 + init_normals];
    DOGL(glGenBuffers(1 + init_normals, vbo_ids));

    // Init vertex VBO
    DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]));
    DOGL(glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3),
                      mesh.vertices.data(), GL_STATIC_DRAW));
    DOGL(glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0));
    DOGL(glEnableVertexAttribArray(vertex_location));
    DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[0]));

    // Init normals VBO
    if (init_normals)
    {
        DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[1]));
        DOGL(glBufferData(GL_ARRAY_BUFFER, mesh.normals->size() * sizeof(float),
                          mesh.normals->data(), GL_STATIC_DRAW));
        DOGL(glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0,
                                   0));
        DOGL(glEnableVertexAttribArray(normal_location));
        DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vbo_ids[1]));
    }

    // Reset VAO bind
    glBindVertexArray(0);
}
