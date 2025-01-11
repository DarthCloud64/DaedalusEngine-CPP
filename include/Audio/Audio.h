//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_AUDIOMANAGER_H
#define DAEDALUSENGINE_AUDIOMANAGER_H

#include <raylib.h>
#include "../Game/MusicComponent.h"

namespace DaedalusEngine {
    void InitializeAudioEngine();
    MusicComponent* CreateMusicComponent();
    void CheckAndPlayMusic(const MusicComponent* musicComponent);
    void PauseMusic(MusicComponent* musicComponent);
    void CleanupAudio();
} // DaedalusEngine

#endif //DAEDALUSENGINE_AUDIOMANAGER_H
