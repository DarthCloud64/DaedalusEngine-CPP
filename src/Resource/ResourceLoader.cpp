//
// Created by neoro on 11/05/2024.
//

#include "../../include/Resource/ResourceLoader.h"

namespace DaedalusEngine {
    ResourceLoader::ResourceLoader() {
        _importer = new Assimp::Importer();
    }

    ResourceLoader::~ResourceLoader() {
        delete _importer;
    }

    void ResourceLoader::LoadModelIntoComponent(std::string file) {
        const aiScene* scene = _importer->ReadFile(file.c_str(), 0);
        printf("debug");
    }
} // DaedalusEngine