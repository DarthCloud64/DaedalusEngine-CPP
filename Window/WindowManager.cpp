//
// Created by neoro on 04/05/2024.
//

#include "WindowManager.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace DaedalusEngine {
    WindowManager::WindowManager(const std::string& windowTitle, int width, int height) {
        printf("Initializing GLFW Window\n");
        _window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    }

    WindowManager::~WindowManager() {
        printf("Killing GLFW Window\n");
        glfwDestroyWindow(_window);
    }

    NativeWindowInformation* WindowManager::GetNativeWindowInformation() {
        HWND win32Window = glfwGetWin32Window(_window);

        NativeWindowInformation* nativeWindowInformation = new NativeWindowInformation();
        nativeWindowInformation->win32Window = win32Window;

        return nativeWindowInformation;
    }

    GLFWwindow *WindowManager::GetAbstractedWindow() {
        return _window;
    }
} // DaedalusEngine