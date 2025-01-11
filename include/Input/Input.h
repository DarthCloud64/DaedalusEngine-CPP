//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_INPUT_H
#define DAEDALUSENGINE_INPUT_H

#include <vector>
#include <raylib.h>

namespace DaedalusEngine {
    enum INPUT {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        ACCEPT,
        PAUSE,
        EXIT_APPLICATION
    };

    std::vector<INPUT> ProcessInputsForCurrentFrame(std::vector<INPUT> currentInputs);
}

#endif //DAEDALUSENGINE_INPUT_H
