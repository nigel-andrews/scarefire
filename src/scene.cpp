#include "scene.hh"

#include <GL/glut.h>

#include "utils.hh"

void Scene::render(ProgramState& state) const
{
    for (auto& collection : this->collections)
    {
        DOGL(glUseProgram(collection.program_id));

        collection.set_uniform(collection);
        collection.render(collection);
    }

    if (this->fire.has_value())
        this->fire->render(state);
}
