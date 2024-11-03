//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_INPUT_H
#define DAEDALUSENGINE_INPUT_H

#include <vector>
#include <GLFW/glfw3.h>

namespace DaedalusEngine {
    enum INPUT {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        ACCEPT,
        PAUSE,
        EXIT_APPLICATION
    };

    std::vector<INPUT> ProcessInputsForCurrentFrame(GLFWwindow* window, std::vector<INPUT> currentInputs);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}

#endif //DAEDALUSENGINE_INPUT_H
