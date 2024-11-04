//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_AUDIOMANAGER_H
#define DAEDALUSENGINE_AUDIOMANAGER_H

#include "../Game/MusicComponent.h"
#include <miniaudio.h>

namespace DaedalusEngine {
    ma_engine* InitializeAudioEngine();
    MusicComponent* CreateMusicComponent(ma_engine* audioEngine);
    void CheckAndPlayMusic(const MusicComponent* musicComponent);
    void PauseMusic(MusicComponent* musicComponent);
    void CleanupAudio(ma_engine* audioEngine);
} // DaedalusEngine

#endif //DAEDALUSENGINE_AUDIOMANAGER_H
