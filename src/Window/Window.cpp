//
// Created by neoro on 04/05/2024.
//

#include "../../include/Window/Window.h"

namespace DaedalusEngine {
    SDL_Window* InitializeWindowEngine(const std::string& windowTitle, int width, int height) {
        printf("Initializing SDL\n");

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL!\n");
        }

        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

        printf("Initializing SDL Window\n");
        return SDL_CreateWindow(windowTitle.c_str(), 0, 0, width, height, windowFlags);
    }

    void CleanupWindowing(SDL_Window* abstractedWindow) {
        SDL_DestroyWindow(abstractedWindow);
    }
} // DaedalusEngine