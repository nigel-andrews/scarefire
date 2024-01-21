#include "scene.hh"

#include <GL/glut.h>

#include "utils.hh"

void Scene::render() const
{
    for (auto& collection : this->collections)
    {
        DOGL(glUseProgram(collection.program_id));

        collection.set_uniform(collection);
        collection.render(collection);
    }
}
