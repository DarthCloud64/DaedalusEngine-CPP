//
// Created by neoro on 05/05/2024.
//

#ifndef DAEDALUSENGINE_RENDERINGMANAGER_H
#define DAEDALUSENGINE_RENDERINGMANAGER_H

#include <memory>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vulkan/vulkan.hpp>
#include "../Window/NativeWindowInformation.h"

namespace DaedalusEngine {

    class RenderingManager {
    private:
        // IEngineFactoryVk* _engineFactoryVk = nullptr;
        // IRenderDevice* _renderDevice = nullptr;
        // IDeviceContext* _deviceContext = nullptr;
        // ISwapChain* _swapChain = nullptr;
        // IPipelineState* _pipelineState = nullptr;
        // IShaderResourceBinding* _shaderResourceBinding = nullptr;
        // IShader* _vertexShader = nullptr;
        // IShader* _fragmentShader = nullptr;
        // IBuffer* _cubeBuffer = nullptr;
        // IBuffer* _cubeIndexBuffer = nullptr;
        VkInstance* _vkInstance;

        // std::tuple<ITextureView*, ITextureView*> SetRenderTargets();
        // void ClearViews(std::tuple<ITextureView*, ITextureView*> renderTargets);
        void CreateVertexShader(std::string fileName, std::string entryPoint);
        void CreateFragmentShader(std::string fileName, std::string entryPoint);
        // IBuffer* CreateVertexBuffer(std::string name, std::vector<Vertex>& data);
        // IBuffer* CreateIndexBuffer(std::string name, std::vector<int>& data);
    public:
        RenderingManager(NativeWindowInformation* nativeWindowInformation);
        ~RenderingManager();
        void Render();
        void Present();
    };

} // DaedalusEngine

#endif //DAEDALUSENGINE_RENDERINGMANAGER_H
