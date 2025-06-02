#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <bx/bx.h>
#include <bx/math.h>

struct TransformComponent
{
    // Current position
    float positionX;
    float positionY;

    // vectors to use when calculating the SRT (scale, rotation, translation) matrix
    bx::Vec3 scale = {0.0f, 0.0f, 0.0f};
    bx::Vec3 rotation = {0.0f, 0.0f, 0.0f};
    bx::Vec3 translation = {0.0f, 0.0f, 0.0f};
};

#endif