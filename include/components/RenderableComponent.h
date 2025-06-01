#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H

#include <bgfx/bgfx.h>

struct RenderableComponent
{
    bgfx::VertexBufferHandle vertexBufferHandle;
    bgfx::IndexBufferHandle indexBufferHandle;
    bgfx::UniformHandle uniformHandle;
    bgfx::ProgramHandle shaderProgram;
    bgfx::TextureHandle texture;
};

#endif