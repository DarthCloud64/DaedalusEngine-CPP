//
// Created by neoro on 04/05/2024.
//

#include "../../include/Input/Input.h"

namespace DaedalusEngine {
    void InitializeInputEngine(GLFWwindow* window) {
        printf("Initializing input engine");
        glfwSetKeyCallback(window, KeyCallback);
    }

    std::vector<INPUT> ProcessInputsForCurrentFrame(GLFWwindow* window, std::vector<INPUT> currentInputs) {

        if(glfwWindowShouldClose(window)){
            currentInputs.push_back(INPUT::EXIT_APPLICATION);
        }

        glfwPollEvents();

        return currentInputs;
    }

    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if(key == GLFW_KEY_P && action == GLFW_PRESS){
            
        }
    }
} // DaedalusEngine