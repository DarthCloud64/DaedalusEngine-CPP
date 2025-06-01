#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

#include "Entity.h"
#include "../components/TransformComponent.h"
#include "../components/RenderableComponent.h"
#include <unordered_map>

struct ComponentStorage
{
    Entity nextEntityId = 0;

    std::unordered_map<Entity, TransformComponent> transformComponents;
    std::unordered_map<Entity, RenderableComponent> renderableComponents;
};

Entity createEntity(ComponentStorage &componentStorage);

#endif