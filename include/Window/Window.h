//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_WINDOWMANAGER_H
#define DAEDALUSENGINE_WINDOWMANAGER_H
#include <iostream>
#include <raylib.h>

namespace DaedalusEngine {
    void InitializeWindowEngine(const std::string& windowTitle, int width, int height);
    void CleanupWindowing();
} // DaedalusEngine

#endif //DAEDALUSENGINE_WINDOWMANAGER_H
