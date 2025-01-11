//
// Created by neoro on 05/05/2024.
//

#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include "../../include/Graphics/Rendering.h"
#include "../../include/Graphics/Vertex.h"
#include "../../include/Utilities/Utilities.h"

namespace DaedalusEngine {
    void InitializeRenderingEngine() {
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(GOLD);
        EndDrawing();
    }

    void CleanupRendering() {
    }
} // DaedalusEngine