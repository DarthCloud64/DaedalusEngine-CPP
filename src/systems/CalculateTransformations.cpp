#include "../../include/systems/CalculateTransformations.h"
#include <iostream>

void calculateViewTransform();
void calculateSRTTransform(std::pair<const Entity, TransformComponent> &transformComponent);

void calculateTransformations(ComponentStorage &componentStorage)
{
    for (std::pair<const Entity, TransformComponent> &transformComponent : componentStorage.transformComponents)
    {
        calculateViewTransform();
        calculateSRTTransform(transformComponent);
    }
}

void calculateViewTransform()
{
    const bx::Vec3 cameraLookAt = {
        0.0f,
        0.0f,
        0.0f,
    };

    const bx::Vec3 cameraPosition = {
        0.0f,
        0.0f,
        -10.0f,
    };

    float viewMatrix[16];
    bx::mtxLookAt(viewMatrix, cameraPosition, cameraLookAt);

    float projectionMatrix[16];
    bx::mtxProj(projectionMatrix, 60.0f, 1200.0f / 720.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, viewMatrix, projectionMatrix);
}

void calculateSRTTransform(std::pair<const Entity, TransformComponent> &transformComponent)
{
    transformComponent.second.scale = {
        1.0f,
        1.0f,
        1.0f,
    };

    transformComponent.second.rotation = {
        0.0f,
        0.0f,
        0.0f,
    };

    transformComponent.second.translation = {
        transformComponent.second.positionX,
        transformComponent.second.positionY,
        0.0f,
    };

    float scaleRotationTranslationMatrix[16];
    bx::mtxIdentity(scaleRotationTranslationMatrix);
    bx::mtxSRT(
        scaleRotationTranslationMatrix,
        transformComponent.second.scale.x,
        transformComponent.second.scale.y,
        transformComponent.second.scale.z,
        transformComponent.second.rotation.x,
        transformComponent.second.rotation.y,
        transformComponent.second.rotation.z,
        transformComponent.second.translation.x,
        transformComponent.second.translation.y,
        transformComponent.second.translation.z);

    bgfx::touch(0);
    bgfx::setTransform(scaleRotationTranslationMatrix);
}