//
// Created by neoro on 11/05/2024.
//

#ifndef DAEDALUSENGINE_RESOURCELOADER_H
#define DAEDALUSENGINE_RESOURCELOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>

namespace DaedalusEngine {

    class ResourceLoader {
    private:
        Assimp::Importer* _importer;
    public:
        ResourceLoader();
        ~ResourceLoader();
        void LoadModelIntoComponent(std::string file);
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_RESOURCELOADER_H
