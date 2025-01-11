//
// Created by neoro on 04/05/2024.
//

#include "../../include/Audio/Audio.h"
#include <cstdio>
#include <stdexcept>

namespace DaedalusEngine {
    void InitializeAudioEngine() {
        printf("Initializing audio engine\n");
        InitAudioDevice();
    }

    MusicComponent* CreateMusicComponent() {
        MusicComponent* musicComponent = new MusicComponent();
        musicComponent->music = LoadMusicStream("/home/rob/assets/corridors.ogg");
        musicComponent->isPlaying = true;
        PlayMusicStream(musicComponent->music);

        return musicComponent;
    }

    void CheckAndPlayMusic(const MusicComponent* musicComponent) {
        if(musicComponent->isPlaying){
            UpdateMusicStream(musicComponent->music);
        }
        else{
            PauseMusicStream(musicComponent->music);
        }
    }

    void PauseMusic(MusicComponent* musicComponent) {
        musicComponent->isPlaying = false;
    }

    void CleanupAudio() {
        CloseAudioDevice();
    }
} // DaedalusEngine