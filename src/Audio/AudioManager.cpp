//
// Created by neoro on 04/05/2024.
//

#include "../../include/Audio/AudioManager.h"
#include <cstdio>

namespace DaedalusEngine {
    AudioManager::AudioManager() {
        printf("Initializing Audio Manager...\n");
        _audioEngine = new ma_engine();

        if(ma_engine_init(nullptr, _audioEngine) != MA_SUCCESS){
            printf("Error initializing miniaudio!");
        }
    }

    void AudioManager::CheckAndPlayMusic(const MusicComponent* musicComponent) {
        if(!ma_sound_is_playing(musicComponent->music)){
            ma_sound_start(musicComponent->music);
        }
    }

    void AudioManager::LoadAudioResourceIntoComponent(const MusicComponent *musicComponent) {
        printf("Loading audio resource\n");

        ma_result result = ma_sound_init_from_file(_audioEngine, "corridors.mp3", 0 , nullptr, nullptr, musicComponent->music);
        if(result != MA_SUCCESS){
            printf("Failed to load music resource file\n");
        }
    }

    void AudioManager::PauseMusic(MusicComponent *musicComponent) {
        ma_sound_stop(musicComponent->music);
    }
} // DaedalusEngine