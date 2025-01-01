//
// Created by neoro on 04/05/2024.
//

#include "../../include/Input/Input.h"

namespace DaedalusEngine {
    std::vector<INPUT> ProcessInputsForCurrentFrame(SDL_Window* window, std::vector<INPUT> currentInputs) {
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p){
                currentInputs.push_back(INPUT::PAUSE);
            }
            if(event.type == SDL_QUIT){
                currentInputs.push_back(INPUT::EXIT_APPLICATION);    
            }
        }

        return currentInputs;
    }
} // DaedalusEngine