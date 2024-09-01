//
// Created by neoro on 04/05/2024.
//

#include "WindowManager.h"

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

namespace DaedalusEngine {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    
    WindowManager::WindowManager(const std::string& windowTitle, int width, int height) {
        printf("Initializing GLFW Window\n");
        _window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    }

    WindowManager::~WindowManager() {
        printf("Killing GLFW Window\n");
        glfwDestroyWindow(_window);
    }

    NativeWindowInformation* WindowManager::GetNativeWindowInformation() {
        Window x11Window = glfwGetX11Window(_window);

        NativeWindowInformation* nativeWindowInformation = new NativeWindowInformation();
        // nativeWindowInformation->win32Window = win32Window;
        nativeWindowInformation->extensions = glfwGetRequiredInstanceExtensions(&nativeWindowInformation->extensionCount);

        return nativeWindowInformation;
    }

    GLFWwindow *WindowManager::GetAbstractedWindow() {
        return _window;
    }
} // DaedalusEngine