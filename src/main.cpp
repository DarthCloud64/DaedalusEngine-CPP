#include <iostream>
#include "../include/Application/Application.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

using namespace DaedalusEngine;

int main(int argc, char *argv[]){
    Application* application = InitializeApplication();
    Run(application);
    return 0;
}
