#include <iostream>
#include "../include/Application/Application.h"

using namespace DaedalusEngine;

int main(int argc, char *argv[]){
    Application* application = InitializeApplication();
    Run(application);
    return 0;
}
