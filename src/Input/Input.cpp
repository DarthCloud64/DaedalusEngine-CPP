//
// Created by neoro on 04/05/2024.
//

#include <iostream>
#include "../../include/Input/Input.h"

namespace DaedalusEngine {
    std::vector<INPUT> ProcessInputsForCurrentFrame(std::vector<INPUT> currentInputs) {
        PollInputEvents();

        if(IsKeyPressed(KEY_P)){
            printf("Paused!\n");
            currentInputs.push_back(INPUT::PAUSE);
        }

        return currentInputs;
    }
} // DaedalusEngine