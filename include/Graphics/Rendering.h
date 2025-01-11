//
// Created by neoro on 05/05/2024.
//

#ifndef DAEDALUSENGINE_RENDERINGMANAGER_H
#define DAEDALUSENGINE_RENDERINGMANAGER_H

#include <memory>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <optional>
#include <raylib.h>

namespace DaedalusEngine {
    struct Rendering {
    };

    void InitializeRenderingEngine();
    void CleanupRendering();
    void Draw();
} // DaedalusEngine/

#endif //DAEDALUSENGINE_RENDERINGMANAGER_H
