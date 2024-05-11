//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_AUDIOCOMPONENT_H
#define DAEDALUSENGINE_AUDIOCOMPONENT_H

#include "Component.h"
#include <miniaudio.h>

namespace DaedalusEngine {
    struct AudioComponent : Component{
        ma_sound* soundEffect = nullptr;
    };
}

#endif //DAEDALUSENGINE_AUDIOCOMPONENT_H
