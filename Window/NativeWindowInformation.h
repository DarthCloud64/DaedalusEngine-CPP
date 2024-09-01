//
// Created by daedalus on 5/6/24.
//

#ifndef DAEDALUSENGINE_NATIVEWINDOWINFORMATION_H
#define DAEDALUSENGINE_NATIVEWINDOWINFORMATION_H

namespace DaedalusEngine {
    struct NativeWindowInformation{
        uint32_t extensionCount;
        const char** extensions;
    };
}

#endif //DAEDALUSENGINE_NATIVEWINDOWINFORMATION_H
