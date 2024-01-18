#include "scene.hh"

void Scene::render() const
{
    for (auto& collection : this->collections)
        collection.render();
}
