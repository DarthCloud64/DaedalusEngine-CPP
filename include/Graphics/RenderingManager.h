//
// Created by neoro on 05/05/2024.
//

#ifndef DAEDALUSENGINE_RENDERINGMANAGER_H
#define DAEDALUSENGINE_RENDERINGMANAGER_H

#include <memory>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <bgfx/bgfx.h>
#include "../../include/Window/NativeWindowInformation.h"


namespace DaedalusEngine {

    class RenderingManager {
    private:
        int width;
        int height;
        void SetRenderTargets();
        void ClearViews();
        void CreateVertexShader(std::string fileName, std::string entryPoint);
        void CreateFragmentShader(std::string fileName, std::string entryPoint);
        /*void CreateVertexBuffer(std::string name, std::vector<Vertex>& data);
        void CreateIndexBuffer(std::string name, std::vector<int>& data);*/
    public:
        explicit RenderingManager(NativeWindowInformation* nativeWindowInformation);
        ~RenderingManager();
        void InitializeEngine(NativeWindowInformation* nativeWindowInformation);
        void InitializeGraphicsPipeline();
        void Render();
        void Present();
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_RENDERINGMANAGER_H
