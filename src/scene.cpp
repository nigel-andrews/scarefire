#include "scene.hh"

#include <GL/glut.h>

void Scene::render() const
{
    for (auto& collection : this->collections)
    {
        DOGL(glUseProgram(collection.program_id));

        collection.set_uniform(collection);
        collection.render(collection);
    }
}
