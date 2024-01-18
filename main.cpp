// clang-format off
#include <cmath>
#include <GL/glew.h>
#include "glm/glm.hpp"

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <functional>
#include <iostream>
#include <vector>

#include "src/collection.hh"
#include "src/mesh.hh"
#include "src/state.hh"
#include "src/utils.hh"

#define DEBUG

#ifdef DEBUG
static std::vector<GLfloat> vertex_buffer_data {
  -0.5, 0.0, +0.5,
  +0.5, 0.0, +0.5,
  +0.5, 0.0, -0.5,
  +0.5, 0.0, -0.5,
  -0.5, 0.0, -0.5,
  -0.5, 0.0, +0.5,
};
#else
static std::vector<GLfloat> vertex_buffer_data {0., 1., 2.};
#endif
// clang-format on

static struct ProgramState _state
{};

GLfloat dot(const GLfloat vec1[3], const GLfloat vec2[3])
{
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

void display()
{
    // _state.scene.model_view_matrix(3, 0) = _state.offset[0];
    // _state.scene.model_view_matrix(3, 1) = _state.offset[1];
    // _state.scene.model_view_matrix(3, 2) = _state.offset[2];

    DOGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // _state.scene.model_view_matrix(0, 3) = _state.offset[0];
    // _state.scene.model_view_matrix(1, 3) = _state.offset[1];
    // _state.scene.model_view_matrix(2, 3) = _state.offset[2];

    _state.scene.render();

    DOGL(glBindVertexArray(0));

    glutSwapBuffers();
}

void anim()
{
    _state.scene.anim_time += 0.1;
    glutPostRedisplay();
}

void timer(int)
{
    anim();
    glutTimerFunc(33, timer, 0);
}

void init_anim()
{
    glutTimerFunc(33, timer, 0);
}

void input_handler(unsigned char key, int, int)
{
    if (key == 'a')
    {
        auto lat = _state.look_at_target;
        GLfloat right[3] = { 1, 0, 0 };
        auto angle =
            std::acos(dot(right, lat) / (dot(lat, lat) * dot(right, right)));

        angle += 0.5;

        _state.look_at_target[0] = std::cos(angle);
        _state.look_at_target[2] = std::sin(angle);
    }
    else if (key == 'd')
    {
    }
    else
    {
        std::cout << "rust > rien\n";
    }
}

void mouse_button_handler(int button, int state, int x, int y)
{
    (void)button;

    if (state == GLUT_DOWN)
    {
        _state.pos[0] = x;
        _state.pos[1] = y;
    }
    else
    {
        _state.held = false;
    }

    auto modifiers = glutGetModifiers();

    _state.shift = modifiers & GLUT_ACTIVE_SHIFT;
    _state.ctrl = modifiers & GLUT_ACTIVE_CTRL;
}

void mouse_motion_handler(int x, int y)
{
    if (!_state.held)
    {
        _state.pos[0] = x;
        _state.pos[1] = y;
        _state.held = true;
    }

    if (_state.shift)
    {
        _state.offset[2] += (_state.pos[1] - y) / 100.;
    }
    else if (_state.ctrl)
    {
        _state.light_pos[0] -= (_state.pos[0] - x) / 1000.;
        _state.light_pos[1] += (_state.pos[1] - y) / 1000.;
    }
    else
    {
        _state.offset[0] -= (_state.pos[0] - x) / 1000.;
        _state.offset[1] += (_state.pos[1] - y) / 1000.;
    }

    _state.pos[0] = x;
    _state.pos[1] = y;

    // Now done in anim
    // glutPostRedisplay();
}

void window_resize(int width, int height)
{
    DOGL(glViewport(0, 0, width, height));
}

void init_glut(int& argc, char* argv[])
{
    // glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shader Programming");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
    glutMouseFunc(mouse_button_handler);
    glutMotionFunc(mouse_motion_handler);
    glutKeyboardFunc(input_handler);
}

bool init_glew()
{
    if (glewInit())
    {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL()
{
    DOGL(glClearDepth(0.0f));
    DOGL(glEnable(GL_DEPTH_TEST));
    DOGL(glDepthFunc(GL_GREATER));
    DOGL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    DOGL(glClearColor(0.4, 0.4, 0.4, 1.0));
    DOGL(glPatchParameteri(GL_PATCH_VERTICES, 4));
}

Collection init_logs()
{
#ifndef DEBUG
    auto log_shader = ShaderConfig{
        .vertex = "shaders/log/vertex.glsl",
        .tesselation_control = "shaders/log/tcs.glsl",
        .tesselation_evaluation = "shaders/log/tes.glsl",
        .geometry = "shaders/log/geometry.glsl",
        .fragment = "shaders/log/fragment.glsl",
    };
#else
    auto log_shader = ShaderConfig{
        .vertex = "shaders/waves/vertex.shd",
        .tesselation_control = "shaders/waves/tcs.glsl",
        .tesselation_evaluation = "shaders/waves/tes.glsl",
        .fragment = "shaders/waves/fragment.shd",
    };
#endif

    MeshData mesh;

    mesh.vertices = vertex_buffer_data;
    // for (size_t i = 0; i < 10; i++)
    // {
    //     for (size_t theta = 0; theta < 360; theta += 30)
    //     {
    //
    //     }
    // }

    std::vector<glm::mat4> transforms;
    transforms.emplace_back(1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0.,
                            0., 0., 1.);

    Collection res(mesh, log_shader, std::move(transforms));

    res.render = std::function<void(const Collection&)>(
        [](const Collection& collection) {
            DOGL(glBindVertexArray(collection.vao_id));
            DOGL(glDrawArrays(GL_PATCHES, 0, 4));
        });

    res.set_uniform =
        std::function<void(const Collection&)>([](const Collection&) {
            // auto shader_id = collection.program_id;

            // SET_UNIFORM(shader_id, "model_view_matrix",
            //             glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
            //                                _state.scene.camera._view));

            // SET_UNIFORM(shader_id, "projection_matrix",
            //             glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
            //                                _state.scene.camera._projection));

            // SET_UNIFORM(shader_id, "anim_time",
            //             glUniform1f(uniform_id, _state.scene.anim_time));

            // SET_UNIFORM(shader_id, "light_pos",
            //             glUniform3fv(uniform_id, 1, _state.light_pos));
        });

    return res;
}

int main(int argc, char* argv[])
{
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();
    init_anim();

    _state.scene.collections.emplace_back(init_logs());

    glutMainLoop();
}
