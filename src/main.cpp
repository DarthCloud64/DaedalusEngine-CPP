#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32 1
#endif

#ifdef __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA 1
#endif

#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11 1
#endif

#include <GLFW/glfw3native.h>

#include "../include/ecs/ComponentStorage.h"
#include "../include/utils/RenderUtils.h"
#include "../include/systems/CalculateTransformations.h"
#include "../include/systems/InitializeDependencies.h"
#include "../include/systems/HandleInput.h"

static const float vertices[] = {
    // X, Y, Z, R, G, B, A, U, V
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.25f, 1.0f, 0.0f, 0.0f,  // top left
};

static const uint16_t indices[] = {
    3, 1, 0, // first triangle
    1, 3, 2  // second triangle
};

static double fps = 1.0 / 60.0;

int main()
{
    GLFWwindow *window = initGlfw();
    ComponentStorage componentStorage{};

    initBgfx(window);

    bgfx::VertexLayout vertexLayout;
    vertexLayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();

    int imageWidth;
    int imageHeight;
    int channels;
    unsigned char *imageData = stbi_load("/home/rob/Pictures/wall.jpg", &imageWidth, &imageHeight, &channels, STBI_rgb_alpha);
    bgfx::TextureFormat::Enum textureFormat = bgfx::TextureFormat::RGBA8;

    Entity triangle = createEntity(componentStorage);

    RenderableComponent renderableComponent{};
    renderableComponent.vertexBufferHandle = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), vertexLayout);
    renderableComponent.indexBufferHandle = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
    renderableComponent.shaderProgram = createProgram("/home/rob/Code/C++/conspiracy/build/vertex.bin", "/home/rob/Code/C++/conspiracy/build/fragment.bin");
    renderableComponent.texture = bgfx::createTexture2D(
        (uint16_t)imageWidth,
        (uint16_t)imageHeight,
        false,
        1,
        textureFormat,
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_POINT,
        bgfx::makeRef(imageData, imageWidth * imageHeight * 4));

    TransformComponent transformComponent{};
    transformComponent.positionX = 0.0f;
    transformComponent.positionY = 0.0f;

    componentStorage.transformComponents[triangle] = transformComponent;
    componentStorage.renderableComponents[triangle] = renderableComponent;

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        glfwPollEvents();

        float velocityY = 0.0f;
        float velocityX = 0.0f;

        if (movingDown)
        {
            std::cout << "down!!!" << std::endl;
            velocityY = -1.0f;
        }
        if (movingUp)
        {
            velocityY = 1.0f;
        }
        if (movingLeft)
        {
            velocityX = -1.0f;
        }
        if (movingRight)
        {
            velocityX = 1.0f;
        }

        componentStorage.transformComponents[triangle].positionY += velocityY * deltaTime;
        componentStorage.transformComponents[triangle].positionX += velocityX * deltaTime;

        calculateTransformations(componentStorage);

        bgfx::setVertexBuffer(0, renderableComponent.vertexBufferHandle);
        bgfx::setIndexBuffer(renderableComponent.indexBufferHandle);
        bgfx::setTexture(0, bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler), renderableComponent.texture);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, renderableComponent.shaderProgram);

        bgfx::frame();
    }

    bgfx::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}