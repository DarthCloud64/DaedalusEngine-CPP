//
// Created by neoro on 04/05/2024.
//

#include "../../include/Window/Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace DaedalusEngine {
    GLFWwindow* InitializeWindowEngine(const std::string& windowTitle, int width, int height) {
        printf("Initializing GLFW Window\n");
        return glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    }

    NativeWindowInformation* GetNativeWindowInformation(GLFWwindow* window) {
        printf("Getting native window information from GLFW window\n");
        HWND win32Window = glfwGetWin32Window(window);

        NativeWindowInformation* nativeWindowInformation = new NativeWindowInformation();
        nativeWindowInformation->win32Window = win32Window;

        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);

        nativeWindowInformation->width = width;
        nativeWindowInformation->height = height;

        return nativeWindowInformation;
    }
} // DaedalusEngine