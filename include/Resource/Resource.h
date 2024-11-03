//
// Created by neoro on 11/05/2024.
//

#ifndef DAEDALUSENGINE_RESOURCELOADER_H
#define DAEDALUSENGINE_RESOURCELOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>

namespace DaedalusEngine {
    void LoadModelIntoComponent(std::string file, Assimp::Importer* importer);
} // DaedalusEngine

#endif //DAEDALUSENGINE_RESOURCELOADER_H
