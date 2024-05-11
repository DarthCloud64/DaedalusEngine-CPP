//
// Created by neoro on 04/05/2024.
//

#ifndef DAEDALUSENGINE_CONTAINER_H
#define DAEDALUSENGINE_CONTAINER_H

#include <vector>
#include "Component.h"

namespace DaedalusEngine {
    struct Container{
        std::vector<Component*> components;
    };
}

#endif //DAEDALUSENGINE_CONTAINER_H
