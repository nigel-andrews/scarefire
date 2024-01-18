#include "collection.hh"

#include <iostream>

#include "shader.hh"
#include "utils.hh"

Collection::Collection(MeshData mesh, struct ShaderConfig shader) {
  GLint shader_program_id = assert_not_neg1(init_arbitrary_shader(shader),
                                            "Failed to initialize shader");

  GLuint vbo_ids[1 + mesh.normals.has_value()];

  GLint vertex_location;
  DOGL(vertex_location = glGetAttribLocation(shader_program_id, "position"));
  // GLint normal_smooth_location; DOGL(normal_smooth_location =
  //          glGetAttribLocation(hair_program_id, "normalSmooth"));
  //
  // DOGL(glGenVertexArrays(1, &_state.vao_id));
  // DOGL(glBindVertexArray(_state.vao_id));
  //
  // if (vertex_location != -1)
  //   nb_vbo++;
  // // if (normal_smooth_location != -1)
  // //   nb_vbo++;
  // DOGL(glGenBuffers(nb_vbo, vbo_ids));
  //
  // if (vertex_location != -1) {
  //   DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]));
  //   DOGL(glBufferData(GL_ARRAY_BUFFER,
  //                     vertex_buffer_data.size() * sizeof(float),
  //                     vertex_buffer_data.data(), GL_STATIC_DRAW));
  //   DOGL(glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0,
  //   0)); DOGL(glEnableVertexAttribArray(vertex_location));
  //   DOGL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1,
  //                         vbo_ids[index_vbo - 1]));
  // }
  // glBindVertexArray(0);
}
