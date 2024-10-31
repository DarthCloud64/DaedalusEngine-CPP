#include <iostream>
#include "../include/Application/Application.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

int main(int argc, char *argv[]){
    DaedalusEngine::Application app;
    app.Run();
    return 0;
}
