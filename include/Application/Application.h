//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_APPLICATION_H
#define DAEDALUSENGINE_APPLICATION_H

#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Game/Container.h"
#include "../Audio/Audio.h"
#include "../Graphics/Rendering.h"
#include "../Resource/Resource.h"
#include "ApplicationState.h"

namespace DaedalusEngine {
    struct Application {
        ApplicationState applicationState;
        NativeWindowInformation* nativeWindowInformation;
        GLFWwindow* abstractedWindow;
        std::vector<INPUT> currentInputs;
        ma_engine* audioEngine;
        Rendering* renderingEngine;
    };

    Application* InitializeApplication();
    void Run(Application* application);
    void Update(Application* application);
    void PresentNextFrame();
    void KillApplication(Application* application);
    void ProcessContainers(Application* application);
    bool WasExitApplicationPressed(const std::vector<INPUT>& whatWasPressed);
} // DaedalusEngine

#endif //DAEDALUSENGINE_APPLICATION_H
