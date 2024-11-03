//
// Created by neoro on 04/05/2024.
//

#include "../../include/Audio/Audio.h"
#include <cstdio>

namespace DaedalusEngine {
    ma_engine* InitializeAudioEngine() {
        printf("Initializing Audio Manager...\n");
        ma_engine* audioEngine = new ma_engine();

        if (ma_engine_init(nullptr, audioEngine) != MA_SUCCESS) {
            printf("Error initializing miniaudio!");
        }

        return audioEngine;
    }

    MusicComponent* CreateMusicComponent(ma_engine* audioEngine) {
        printf("Creating music component\n");

        MusicComponent* musicComponent = new MusicComponent();
        ma_result result = ma_sound_init_from_file(audioEngine, "corridors.mp3", 0 , nullptr, nullptr, musicComponent->music);
        if(result != MA_SUCCESS){
            printf("Failed to load music resource file\n");
        }

        return musicComponent;
    }

    void CheckAndPlayMusic(const MusicComponent* musicComponent) {
        if (!ma_sound_is_playing(musicComponent->music)) {
            ma_sound_start(musicComponent->music);
        }
    }

    void PauseMusic(MusicComponent* musicComponent) {
        ma_sound_stop(musicComponent->music);
    }
} // DaedalusEngine