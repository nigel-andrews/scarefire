#pragma once

#include <GL/glew.h>
#include <functional>
#include <stdexcept>
#include <vector>

#include "mesh.hh"
#include "shader.hh"
#include "utils.hh"

struct Collection
{
    // Shared
    GLuint vao_id;
    GLuint program_id;
    MeshData mesh_;
    ShaderConfig shaders_;

    // Individual
    std::vector<Mat<4>> transforms;

    // Aggregate
    std::function<void(const Collection&)> render = [](const Collection&) {
        throw std::runtime_error("[C++][FATAL] Unimplemented render().\n");
    };

    std::function<void(const Collection&)> set_uniform = [](const Collection&) {
        throw std::runtime_error("[C++][FATAL] Unimplemented set_uniform().\n");
    };

    // Methods
    Collection(MeshData mesh, ShaderConfig shader,
               std::vector<Mat<4>>&& transforms);

    //! ================================= END =================================
    //! Anything below this is kept in case we have enough time to complete it.

    // Loading from file, no reason to do it this was just a waste of time...
    // Collection(std::string path) {
    //   std::fstream file(path, std::ios::in);
    //   if (!file)
    //     throw std::runtime_error(std::format("File not found: {}", path));
    //
    //   std::string line;
    //   std::getline(file, line);
    //   if (file.fail())
    //     throw std::runtime_error(std::format("Error reading file: {}",
    //     path));
    //
    //   size_t nb_vbo = 1;
    //   bool normals = false;
    //   bool uvs = false;
    //   for (char c : line) {
    //     switch (c) {
    //     case 'n':
    //       nb_vbo++;
    //       normals = true;
    //       break;
    //     case 't':
    //       nb_vbo++;
    //       uvs = true;
    //     default:
    //       throw std::runtime_error(
    //           std::format("Unknown buffer data type: {}", c));
    //     }
    //   }
    //
    //   std::vector<GLfloat> vertex_buffer{};
    //   std::vector<GLfloat> normal_buffer{};
    //   std::vector<GLfloat> uv_buffer{};
    //
    //   auto buffers = {vertex_buffer, normal_buffer, uv_buffer};
    //
    //   size_t index = 1;
    //   size_t vbo_index = 0;
    //   for (; std::getline(file, line).good(); index++) {
    //     if (line.empty()) {
    //       if (vbo_index == nb_vbo - 1)
    //         throw std::runtime_error(
    //             std::format("Too many empty lines in file: {}", path));
    //       vbo_index++;
    //       continue;
    //     }
    //
    //     std::vector<std::string> coords = split(line, ',');
    //     if (coords.size() == 1 || coords.size() > 4)
    //       throw std::runtime_error(
    //           std::format("Invalid vector size {} at line {} of {}",
    //                       coords.size(), index, path));
    //
    //     for (const auto &coord : coords) {
    //       std::stof(coord);
    //     }
    //   }
    //
    //   if (file.fail())
    //     throw std::runtime_error("Error reading file: " + path);
    //
    //   // int max_nb_vbo = 5;
    //   // int nb_vbo = 0;
    //   // int index_vbo = 0;
    //   // GLuint vbo_ids[max_nb_vbo];
    //   //
    //   // GLint vertex_location;
    //   // DOGL(vertex_location =
    //   //          glGetAttribLocation(_state.log_program_id, "position"));
    //   // // GLint normal_smooth_location; DOGL(normal_smooth_location =
    //   // //          glGetAttribLocation(hair_program_id, "normalSmooth"));
    //   //
    //   // DOGL(glGenVertexArrays(1, &_state.vao_id));
    //   // DOGL(glBindVertexArray(_state.vao_id));
    //   //
    //   // if (vertex_location != -1)
    //   //   nb_vbo++;
    //   // // if (normal_smooth_location != -1)
    //   // //   nb_vbo++;
    //   // DOGL(glGenBuffers(nb_vbo, vbo_ids));
    //   //
    //   // if (vertex_location != -1) {
    //   //   DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]));
    //   //   DOGL(glBufferData(GL_ARRAY_BUFFER,
    //   //                     vertex_buffer_data.size() * sizeof(float),
    //   //                     vertex_buffer_data.data(), GL_STATIC_DRAW));
    //   //   DOGL(glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE,
    //   0,
    //   //   0)); DOGL(glEnableVertexAttribArray(vertex_location));
    //   //   DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1,
    //   //                         vbo_ids[index_vbo - 1]));
    //   // }
    //   // glBindVertexArray(0);
    // }
};
