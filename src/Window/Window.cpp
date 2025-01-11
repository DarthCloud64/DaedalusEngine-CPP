//
// Created by neoro on 04/05/2024.
//

#include "../../include/Window/Window.h"

namespace DaedalusEngine {
    void InitializeWindowEngine(const std::string& windowTitle, int width, int height) {
        printf("Initializing windowing engine\n");

        InitWindow(width, height, windowTitle.c_str());
    }

    void CleanupWindowing() {
        CloseWindow();
    }
} // DaedalusEngine