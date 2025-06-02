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

static bool movingUp = false;
static bool movingDown = false;
static bool movingLeft = false;
static bool movingRight = false;

static void key_callback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_DOWN)
    {
        movingDown = true;
    }
    else
    {
        movingDown = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_UP)
    {
        movingUp = true;
    }
    else
    {
        movingUp = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_LEFT)
    {
        movingLeft = true;
    }
    else
    {
        movingLeft = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_RIGHT)
    {
        movingRight = true;
    }
    else
    {
        movingRight = false;
    }
}

static double fps = 1.0 / 60.0;

int main()
{
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW failed to initialize\n");
    }

    ComponentStorage componentStorage{};

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(1200, 720, "Test", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);

    bgfx::Init init;
    init.type = bgfx::RendererType::Vulkan;
    init.platformData.nwh = (void *)glfwGetX11Window(window);
    init.platformData.ndt = glfwGetX11Display();
    init.resolution.width = 1200;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

    uint8_t red = 154;
    uint8_t green = 100;
    uint8_t blue = 73;
    uint8_t alpha = 255;
    uint32_t rbgaHex = (red << 24) | (green << 16) | (blue << 8) | alpha;
    bgfx::setViewRect(0, 0, 0, uint16_t(1200), uint16_t(720));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, rbgaHex, 1.0f, 0);

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

        transformComponent.positionY += velocityY * deltaTime;
        transformComponent.positionX += velocityX * deltaTime;

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

        transformComponent.scale = {
            1.0f,
            1.0f,
            1.0f,
        };

        transformComponent.rotation = {
            0.0f,
            0.0f,
            0.0f,
        };

        transformComponent.translation = {
            transformComponent.positionX,
            transformComponent.positionY,
            0.0f,
        };

        float scaleRotationTranslationMatrix[16];
        bx::mtxIdentity(scaleRotationTranslationMatrix);
        bx::mtxSRT(
            scaleRotationTranslationMatrix,
            transformComponent.scale.x,
            transformComponent.scale.y,
            transformComponent.scale.z,
            transformComponent.rotation.x,
            transformComponent.rotation.y,
            transformComponent.rotation.z,
            transformComponent.translation.x,
            transformComponent.translation.y,
            transformComponent.translation.z);

        bgfx::touch(0);
        bgfx::setTransform(scaleRotationTranslationMatrix);

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