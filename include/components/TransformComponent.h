#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>
using namespace glm;

struct TransformComponent
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

#endif