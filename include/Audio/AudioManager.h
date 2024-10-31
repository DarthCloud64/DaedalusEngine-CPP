//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_AUDIOMANAGER_H
#define DAEDALUSENGINE_AUDIOMANAGER_H

#include "../Game/MusicComponent.h"
#include <miniaudio.h>

namespace DaedalusEngine {

    class AudioManager {
    private:
        ma_engine* _audioEngine;
    public:
        AudioManager();
        void LoadAudioResourceIntoComponent(const MusicComponent* musicComponent);
        void CheckAndPlayMusic(const MusicComponent* musicComponent);
        void PauseMusic(MusicComponent* musicComponent);
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_AUDIOMANAGER_H
