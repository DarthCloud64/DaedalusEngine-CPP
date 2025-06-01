#include "../../include/ecs/ComponentStorage.h"

Entity createEntity(ComponentStorage &componentStorage)
{
    return componentStorage.nextEntityId++;
}