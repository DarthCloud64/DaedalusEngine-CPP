#ifndef HANDLEINPUT_H
#define HANDLEINPUT_H
#include <GLFW/glfw3.h>

extern bool movingUp;
extern bool movingDown;
extern bool movingLeft;
extern bool movingRight;

void key_callback(GLFWwindow *window, int key, int scanCode, int action, int mods);

#endif