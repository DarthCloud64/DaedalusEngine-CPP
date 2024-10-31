//
// Created by neoro on 04/05/2024.
//

#include "../../include/Input/InputManager.h"

namespace DaedalusEngine {
    std::vector<INPUT> InputManager::_currentInputs;

    InputManager::InputManager(GLFWwindow *window) {
        _window = window;
        glfwSetKeyCallback(_window, KeyCallback);
    }

    std::vector<INPUT> InputManager::ProcessInputsForCurrentFrame() {

        if(glfwWindowShouldClose(_window)){
            _currentInputs.push_back(INPUT::EXIT_APPLICATION);
        }

        glfwPollEvents();

        return _currentInputs;
    }

    void InputManager::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if(key == GLFW_KEY_P && action == GLFW_PRESS){
            _currentInputs.push_back(INPUT::PAUSE);
        }
    }

    void InputManager::ClearCurrentInputs() {
        _currentInputs.clear();
    }
} // DaedalusEngine