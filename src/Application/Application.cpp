//
// Created by neoro on 04/05/2024.
//

#include <iostream>
#include "../../include/Application/Application.h"
#include <vector>

namespace DaedalusEngine {
    //temporary containers. move these into scenes
    Container* _music_Container;

    Application* InitializeApplication() {
        Application* application = new Application();

        printf("Initializing application\n");
        application->applicationState = ApplicationState::RUNNING;

        application->abstractedWindow = InitializeWindowEngine("Daedalus Engine", 1200, 720);
        application->audioEngine = InitializeAudioEngine();
        application->renderingEngine = InitializeRenderingEngine(GetNativeWindowInformation(application->abstractedWindow));

        // TODO: this (and below) is just for testing. Remove later
        printf("Initializing test component\n");
        MusicComponent* musicComponent = CreateMusicComponent(application->audioEngine);
        musicComponent->isPlaying = true;

        printf("Initializing test container\n");
        _music_Container = new Container();
        _music_Container->components.push_back(musicComponent);

        return application;
    }

    void KillApplication(Application* application) {
        printf("Killing test components\n");
        for (Component* entry : _music_Container->components) {
            if (dynamic_cast<MusicComponent*>(entry) != nullptr) {
                ma_sound_uninit(dynamic_cast<MusicComponent*>(entry)->music);
            }
            delete entry;
        }

        printf("Killing test container\n");
        delete _music_Container;

        printf("Killing Audio\n");
        CleanupAudio(application->audioEngine);

        printf("Killing Windowing\n");
        CleanupWindowing(application->abstractedWindow);
        
        printf("Killing Rendering\n");
        CleanupRendering(application->renderingEngine);
        delete application->renderingEngine;

        printf("Killing remainder of application\n");
        delete application->nativeWindowInformation;
        delete application;
    }

    void Run(Application* application) {
        printf("Running application...");

        int count = 0;
        while(application->applicationState != ApplicationState::KILLED){
            Update(application);
            PresentNextFrame();
        }

        KillApplication(application);
    }

    void Update(Application* application) {
        ProcessContainers(application);

        std::vector<INPUT> whatWasPressed = ProcessInputsForCurrentFrame(application->abstractedWindow, application->currentInputs);
        bool wasExitPressed = WasExitApplicationPressed(whatWasPressed);
        if(wasExitPressed){
            application->applicationState = ApplicationState::KILLED;
        }

        for(INPUT entry : whatWasPressed){
            if(entry == INPUT::PAUSE){
                if(application->applicationState == ApplicationState::PAUSED){
                    application->applicationState = ApplicationState::RUNNING;
                } else{
                    application->applicationState = ApplicationState::PAUSED;
                }
            }
        }

        application->currentInputs.clear();

        Render();
    }

    bool WasExitApplicationPressed(const std::vector<INPUT>& whatWasPressed) {
        for(INPUT i : whatWasPressed){
            if(i == INPUT::EXIT_APPLICATION){
                return true;
            }
        }

        return false;
    }

    void PresentNextFrame() {
        Present();
    }

    // TODO: move this logic into a scene class
    void ProcessContainers(Application* application) {
        for (Component *component: _music_Container->components) {
            if (dynamic_cast<MusicComponent *>(component) != nullptr) {
                if(application->applicationState != ApplicationState::PAUSED) {
                    CheckAndPlayMusic(dynamic_cast<MusicComponent *>(component));
                } else{
                    PauseMusic(dynamic_cast<MusicComponent*>(component));
                }
            }
        }
    }
} // DaedalusEngine