#include <iostream>

#include "../../include/systems/InitializeDependencies.h"

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
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/platform.h>
#include "../include/systems/HandleInput.h"

GLFWwindow *initGlfw()
{
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW failed to initialize\n");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow *window = glfwCreateWindow(1200, 720, "Test", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

void initBgfx(GLFWwindow *window)
{
    bgfx::Init init;
    init.type = bgfx::RendererType::Vulkan;
    init.platformData.nwh = (void *)glfwGetX11Window(window);
    init.platformData.ndt = glfwGetX11Display();
    init.resolution.width = 1200;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);
    bgfx::setDebug(BGFX_DEBUG_STATS);

    uint8_t red = 154;
    uint8_t green = 100;
    uint8_t blue = 73;
    uint8_t alpha = 255;
    uint32_t rbgaHex = (red << 24) | (green << 16) | (blue << 8) | alpha;
    bgfx::setViewRect(0, 0, 0, uint16_t(1200), uint16_t(720));
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, rbgaHex, 1.0f, 0);
}