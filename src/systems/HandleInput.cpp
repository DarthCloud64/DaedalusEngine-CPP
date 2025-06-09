#include "../../include/systems/HandleInput.h"
#include <iostream>

bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;

void key_callback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_DOWN)
    {
        movingDown = true;
    }
    else
    {
        movingDown = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_UP)
    {
        movingUp = true;
    }
    else
    {
        movingUp = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_LEFT)
    {
        movingLeft = true;
    }
    else
    {
        movingLeft = false;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_RIGHT)
    {
        movingRight = true;
    }
    else
    {
        movingRight = false;
    }
}
