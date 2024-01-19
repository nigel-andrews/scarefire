// clang-format off
#include <GL/glew.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include <functional>
#include <iostream>
#include <vector>

#include "glm/ext/scalar_constants.hpp"
#include "src/collection.hh"
#include "src/mesh.hh"
#include "src/state.hh"
#include "src/utils.hh"

#define DEBUG
#undef DEBUG

#ifdef DEBUG
static std::vector<glm::vec3> vertex_buffer_data {
    {-0.5, 0.0, +0.5},
    {+0.5, 0.0, +0.5},
    {+0.5, 0.0, -0.5},
    {+0.5, 0.0, -0.5},
    {-0.5, 0.0, -0.5},
    {-0.5, 0.0, +0.5},
};
#else
static std::vector<glm::vec3> vertex_buffer_data{
        {0., 0., 0.},
        {0., 1., 0.},
        {1., 1., 0.},
        {1., 0., 0.}
};
static glm::vec3 log_center{0.5, 0.5, 0.};
#endif
// clang-format on

static struct ProgramState _state
{};

void display()
{
    DOGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

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

void process_standard_keys(unsigned char key, int, int)
{
    Camera& camera = _state.scene.camera;
    float delta_time =
        std::max(_state.scene.anim_time - _state.prev_input_time, 1e-3f);

    {
        glm::vec3 movement = {};
        if (key == 'w')
            movement += camera.forward() / delta_time;
        if (key == 's')
            movement -= camera.forward() / delta_time;
        if (key == 'd')
            movement += camera.right() / delta_time;
        if (key == 'a')
            movement -= camera.right() / delta_time;

        std::cout << movement.x << ' ' << movement.y << std::endl;

        float speed = 0.1f;

        if (movement.length() > 0.0f)
        {
            const glm::vec3 new_pos =
                camera.position() + movement * delta_time * speed;

            camera.set_view(
                glm::lookAt(new_pos, new_pos + camera.forward(), camera.up()));

            std::cout << "Pos: " << new_pos.x << ' ' << new_pos.y << std::endl;

            glutPostRedisplay();
        }
    }

    _state.prev_input_time = _state.scene.anim_time;
}

void mouse_button_handler(int button, int state, int x, int y)
{
    (void)button;

    if (state == GLUT_DOWN)
        _state.mouse_pos = { x, y };
    else
        _state.held = false;

    auto modifiers = glutGetModifiers();
    _state.ctrl = modifiers & GLUT_ACTIVE_CTRL;
}

void mouse_motion_handler(int x, int y)
{
    if (!_state.held)
    {
        // Resync mouse pos if it wasn't held
        _state.mouse_pos = { x, y };
        _state.held = true;
    }

    if (_state.ctrl)
    {
        _state.light_pos -=
            _state.scene.camera.right() * ((_state.mouse_pos[0] - x) / 1000.f);
        _state.light_pos +=
            _state.scene.camera.up() * ((_state.mouse_pos[1] - y) / 1000.f);
    }
    else
    {
        Camera& camera = _state.scene.camera;

        const glm::vec2 new_pos{ x, y };
        const glm::vec2 delta = glm::vec2(_state.mouse_pos - new_pos) * 0.01f;
        if (delta.length() > 0.0f)
        {
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), delta.x,
                                        glm::vec3(0.0f, 1.0f, 0.0f));
            rot = glm::rotate(rot, delta.y, camera.right());
            camera.set_view(glm::lookAt(
                camera.position(),
                camera.position() + (glm::mat3(rot) * camera.forward()),
                (glm::mat3(rot) * camera.up())));
        }
    }

    _state.mouse_pos = { x, y };
}

void window_resize(int width, int height)
{
    _state.scene.camera.set_ratio(float(width) / float(height));
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
    glutKeyboardFunc(process_standard_keys);
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

    std::vector<glm::mat4> transforms;
    // clang-format off
    auto transform = glm::mat4(1.);


    transform = glm::translate(transform, {1, 0, 0});
    transform = glm::rotate(transform, glm::pi<float>() / 4.f, {1, 0, 0});
    transforms.emplace_back(transform);
    transform = glm::rotate(transform, -glm::pi<float>() / 4.f, {1, 0, 0});

    transform = glm::rotate(transform, glm::pi<float>() / 2.f , {0,1,0});
    transforms.emplace_back(transform);
    transform = glm::rotate(transform, -glm::pi<float>() / 4.f, {1, 0, 0});

    transform = glm::rotate(transform, glm::pi<float>() / 2.f , {0,1,0});
    transforms.emplace_back(transform);
    transform = glm::rotate(transform, -glm::pi<float>() / 4.f, {1, 0, 0});

    transform = glm::rotate(transform, glm::pi<float>() / 2.f , {0,1,0});
    transforms.emplace_back(transform);
    transform = glm::rotate(transform, -glm::pi<float>() / 4.f, {1, 0, 0});
    // clang-format on

    Collection res(mesh, log_shader, std::move(transforms));

    res.render = std::function<void(const Collection&)>(
        [](const Collection& collection) {
            DOGL(glBindVertexArray(collection.vao_id));

            for (const auto& transform : collection.transforms)
            {
                SET_UNIFORM(collection.program_id, "model",
                            glUniformMatrix4fv(uniform_id, 1, GL_FALSE,
                                               (const float*)&transform));
                DOGL(glDrawArrays(GL_PATCHES, 0, 4));
            }
        });

    res.set_uniform = std::function<void(const Collection&)>(
        [](const Collection& collection) {
            auto shader_id = collection.program_id;

            SET_UNIFORM(shader_id, "view_proj",
                        glUniformMatrix4fv(
                            uniform_id, 1, GL_FALSE,
                            (const float*)&_state.scene.camera._view_proj));

            SET_UNIFORM(
                shader_id, "light_pos",
                glUniform3fv(uniform_id, 1, (const float*)&_state.light_pos));
#ifdef DEBUG
            SET_UNIFORM(shader_id, "anim_time",
                        glUniform1f(uniform_id, _state.scene.anim_time));
#else
            SET_UNIFORM(shader_id, "log_depth",
                        glUniform1f(uniform_id, _state.log_depth));
            SET_UNIFORM(shader_id, "log_width",
                        glUniform1f(uniform_id, _state.log_width));
            SET_UNIFORM(shader_id, "log_center",
                        glUniform3fv(uniform_id, 1, (const float*)&log_center));
#endif
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
