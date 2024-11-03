//
// Created by neoro on 11/05/2024.
//

#include "../../include/Resource/Resource.h"

namespace DaedalusEngine {
    void LoadModelIntoComponent(std::string file, Assimp::Importer* importer) {
        const aiScene* scene = importer->ReadFile(file.c_str(), 0);
    }
} // DaedalusEngine