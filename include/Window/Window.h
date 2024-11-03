//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_WINDOWMANAGER_H
#define DAEDALUSENGINE_WINDOWMANAGER_H
#include <iostream>
#include "NativeWindowInformation.h"
#include <GLFW/glfw3.h>

namespace DaedalusEngine {
    GLFWwindow* InitializeWindowEngine(const std::string& windowTitle, int width, int height);
    NativeWindowInformation* GetNativeWindowInformation(GLFWwindow* window);
} // DaedalusEngine

#endif //DAEDALUSENGINE_WINDOWMANAGER_H
