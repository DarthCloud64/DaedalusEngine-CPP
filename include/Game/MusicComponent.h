//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_MUSICCOMPONENT_H
#define DAEDALUSENGINE_MUSICCOMPONENT_H

#include "Component.h"

namespace DaedalusEngine {
    struct MusicComponent : Component{
        Music music;
        bool isPlaying = false;
    };
}

#endif //DAEDALUSENGINE_MUSICCOMPONENT_H
