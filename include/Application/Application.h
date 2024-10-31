//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_APPLICATION_H
#define DAEDALUSENGINE_APPLICATION_H

#include "../Window/WindowManager.h"
#include "../Input/InputManager.h"
#include "../Input/Input.h"
#include "../Game/Container.h"
#include "../Audio/AudioManager.h"
#include "../Graphics/RenderingManager.h"
#include "../Resource/ResourceLoader.h"
#include "ApplicationState.h"

namespace DaedalusEngine {

    class Application {
    private:
        ApplicationState _state;
        NativeWindowInformation* _nativeWindowInformation;
        WindowManager* _windowManager;
        InputManager* _inputManager;
        AudioManager* _audioManager;
        RenderingManager*_renderingManager;
        ResourceLoader* _resourceLoader;

        //temporary containers. move these into scenes
        Container* _music_Container;

        bool WasExitApplicationPressed(const std::vector<INPUT>& whatWasPressed);
    public:
        Application();
        ~Application();
        void Run();
        void Update();
        void PresentNextFrame();
        void InitializeExternalDependencies();
        void KillExternalDependencies();
        void ProcessContainers();
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_APPLICATION_H
