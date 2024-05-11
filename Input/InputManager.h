//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_INPUTMANAGER_H
#define DAEDALUSENGINE_INPUTMANAGER_H

#include <vector>
#include <GLFW/glfw3.h>
#include "Input.h"

namespace DaedalusEngine {

    class InputManager {
    private:
        GLFWwindow* _window;
    public:
        static std::vector<INPUT> _currentInputs;
        explicit InputManager(GLFWwindow* window);
        std::vector<INPUT> ProcessInputsForCurrentFrame();
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void ClearCurrentInputs();
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_INPUTMANAGER_H
