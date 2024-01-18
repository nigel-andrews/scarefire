#include "scene.hh"

#include <GL/glut.h>

void Scene::render() const
{
    DOGL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    for (auto& collection : this->collections)
    {
        DOGL(glUseProgram(collection.program_id));

        collection.set_uniform(collection);
        collection.render(collection);
    }

    DOGL(glBindVertexArray(0));

    glutSwapBuffers();
}
