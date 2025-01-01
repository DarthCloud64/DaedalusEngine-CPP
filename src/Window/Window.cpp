//
// Created by neoro on 04/05/2024.
//

#include "../../include/Window/Window.h"

namespace DaedalusEngine {
    SDL_Window* InitializeWindowEngine(const std::string& windowTitle, int width, int height) {
        printf("Initializing SDL\n");

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Failed to initialize SDL!\n");
        }

        printf("Initializing SDL Window\n");
        return SDL_CreateWindow(windowTitle.c_str(), 0, 0, width, height, SDL_WINDOW_SHOWN);
    }

    NativeWindowInformation* GetNativeWindowInformation(SDL_Window* window) {
        printf("Getting native window information from SDL window\n");
        // HWND win32Window = glfwGetWin32Window(window);

        NativeWindowInformation* nativeWindowInformation = new NativeWindowInformation();

        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);

        nativeWindowInformation->width = width;
        nativeWindowInformation->height = height;

        return nativeWindowInformation;
    }

    void CleanupWindowing(GLFWwindow* glfwWindow) {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }
} // DaedalusEngine