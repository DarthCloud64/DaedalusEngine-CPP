//
// Created by neoro on 04/05/2024.
//

#include <iostream>
#include "Application.h"
#include <vector>

namespace DaedalusEngine {
    Application::Application() {
        printf("Initializing application\n");
        InitializeExternalDependencies();
        _state = ApplicationState::RUNNING;

        printf("Initializing managers\n");
        _windowManager = new WindowManager("Daedalus Engine", 1200, 720);
        _inputManager = new InputManager(_windowManager->GetAbstractedWindow());
        _audioManager = new AudioManager();

        _nativeWindowInformation = _windowManager->GetNativeWindowInformation();
        _renderingManager = new RenderingManager(_nativeWindowInformation);

        _resourceLoader = new ResourceLoader();

        // TODO: this (and below) is just for testing. Remove later
        printf("Initializing test component\n");
        MusicComponent* musicComponent = new MusicComponent();
        _audioManager->LoadAudioResourceIntoComponent(musicComponent);
        musicComponent->isPlaying = true;

        printf("Initializing test container\n");
        _music_Container = new Container();
        _music_Container->components.push_back(musicComponent);
    }

    Application::~Application() {
        printf("Killing application\n");
        KillExternalDependencies();

        printf("Killing test components\n");
        for(Component* entry : _music_Container->components){
            if(dynamic_cast<MusicComponent*>(entry) != nullptr){
                ma_sound_uninit(dynamic_cast<MusicComponent*>(entry)->music);
            }
            delete entry;
        }

        printf("Killing test container\n");
        delete _music_Container;

        printf("Killing managers\n");
        delete _renderingManager;
        delete _windowManager;
        delete _inputManager;
        delete _audioManager;
        delete _nativeWindowInformation;
    }

    void Application::InitializeExternalDependencies() {
        printf("Initializing GLFW\n");

        if(!glfwInit()){
            printf("Failed to initialize GLFW!");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void Application::KillExternalDependencies() {
        printf("Killing GLFW\n");
        glfwTerminate();
    }

    void Application::Run() {
        int count = 0;
        while(_state != ApplicationState::KILLED){
            Update();
            PresentNextFrame();
        }
    }

    void Application::Update() {
        ProcessContainers();

        std::vector<INPUT> whatWasPressed = _inputManager->ProcessInputsForCurrentFrame();
        bool wasExitPressed = WasExitApplicationPressed(whatWasPressed);
        if(wasExitPressed){
            _state = ApplicationState::KILLED;
        }

        for(INPUT entry : whatWasPressed){
            if(entry == INPUT::PAUSE){
                if(_state == ApplicationState::PAUSED){
                    _state = ApplicationState::RUNNING;
                } else{
                    _state = ApplicationState::PAUSED;
                }
            }
        }
        DaedalusEngine::InputManager::ClearCurrentInputs();

        _renderingManager->Render();
    }

    bool Application::WasExitApplicationPressed(const std::vector<INPUT>& whatWasPressed) {
        for(INPUT i : whatWasPressed){
            if(i == INPUT::EXIT_APPLICATION){
                return true;
            }
        }

        return false;
    }

    void Application::PresentNextFrame() {
        _renderingManager->Present();
    }

    // TODO: move this logic into a scene class
    void Application::ProcessContainers() {
        for (Component *component: _music_Container->components) {
            if (dynamic_cast<MusicComponent *>(component) != nullptr) {
                if(_state != ApplicationState::PAUSED) {
                    _audioManager->CheckAndPlayMusic(dynamic_cast<MusicComponent *>(component));
                } else{
                    _audioManager->PauseMusic(dynamic_cast<MusicComponent*>(component));
                }
            }
        }
    }
} // DaedalusEngine