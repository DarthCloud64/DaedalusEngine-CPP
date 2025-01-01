//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_WINDOWMANAGER_H
#define DAEDALUSENGINE_WINDOWMANAGER_H
#include <iostream>
#include "NativeWindowInformation.h"
#include <SDL2/SDL.h>

namespace DaedalusEngine {
    SDL_Window* InitializeWindowEngine(const std::string& windowTitle, int width, int height);
    NativeWindowInformation* GetNativeWindowInformation(SDL_Window* window);
    void CleanupWindowing(SDL_Window* glfwWindow);
} // DaedalusEngine

#endif //DAEDALUSENGINE_WINDOWMANAGER_H
