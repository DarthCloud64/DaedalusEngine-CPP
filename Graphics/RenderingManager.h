//
// Created by neoro on 05/05/2024.
//

#ifndef DAEDALUSENGINE_RENDERINGMANAGER_H
#define DAEDALUSENGINE_RENDERINGMANAGER_H

#include <memory>
#include <iomanip>
#include <iostream>
#include <tuple>

#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <Windows.h>
#include <crtdbg.h>

#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#ifndef ENGINE_DLL
#    define ENGINE_DLL 1
#endif

#ifndef D3D11_SUPPORTED
#    define D3D11_SUPPORTED 1
#endif

#ifndef D3D12_SUPPORTED
#    define D3D12_SUPPORTED 1
#endif

#ifndef GL_SUPPORTED
#    define GL_SUPPORTED 1
#endif

#ifndef VULKAN_SUPPORTED
#    define VULKAN_SUPPORTED 1
#endif

#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include <Platforms/Win32/interface/Win32NativeWindow.h>
#include "../Window/NativeWindowInformation.h"

using namespace Diligent;

namespace DaedalusEngine {

    class RenderingManager {
    private:
        IEngineFactoryVk* _engineFactoryVk = nullptr;
        IRenderDevice* _renderDevice = nullptr;
        IDeviceContext* _deviceContext = nullptr;
        ISwapChain* _swapChain = nullptr;
        IPipelineState* _pipelineState = nullptr;
        IShaderResourceBinding* _shaderResourceBinding = nullptr;
        IShader* _vertexShader = nullptr;
        IShader* _fragmentShader = nullptr;
        IBuffer* _cubeBuffer = nullptr;
        IBuffer* _cubeIndexBuffer = nullptr;

        std::tuple<ITextureView*, ITextureView*> SetRenderTargets();
        void ClearViews(std::tuple<ITextureView*, ITextureView*> renderTargets);
        void CreateVertexShader(std::string fileName, std::string entryPoint);
        void CreateFragmentShader(std::string fileName, std::string entryPoint);
        IBuffer* CreateVertexBuffer(std::string name, std::vector<float>& data);
        IBuffer* CreateIndexBuffer(std::string name, std::vector<int>& data);
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
