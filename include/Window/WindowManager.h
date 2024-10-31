//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_WINDOWMANAGER_H
#define DAEDALUSENGINE_WINDOWMANAGER_H
#include <iostream>
#include "NativeWindowInformation.h"
#include <GLFW/glfw3.h>

namespace DaedalusEngine {

    class WindowManager {
    private:
        GLFWwindow* _window = nullptr;
    public:
        WindowManager(const std::string& windowTitle, int width, int height);
        ~WindowManager();
        GLFWwindow* GetAbstractedWindow();
        NativeWindowInformation* GetNativeWindowInformation();
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_WINDOWMANAGER_H
