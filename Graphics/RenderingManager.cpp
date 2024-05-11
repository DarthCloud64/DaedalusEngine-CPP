//
// Created by neoro on 05/05/2024.
//

#include "RenderingManager.h"

namespace DaedalusEngine {
    RenderingManager::RenderingManager(NativeWindowInformation* nativeWindowInformation) {
        InitializeEngine(nativeWindowInformation);
    }

    RenderingManager::~RenderingManager() {

    }

    void RenderingManager::InitializeEngine(NativeWindowInformation* nativeWindowInformation) {
#if VULKAN_SUPPORTED
        EngineVkCreateInfo engineVkCreateInfo;
        SwapChainDesc swapChainDesc;

#ifdef EXPLICITLY_LOAD_ENGINE_VK_DLL
        auto GetEngineFactoryVk = LoadGraphicsEngineVk();
#endif
        Win32NativeWindow win32NativeWindow;
        win32NativeWindow.hWnd = nativeWindowInformation->win32Window;

        IEngineFactoryVk* engineFactoryVk = GetEngineFactoryVk();
        engineFactoryVk->CreateDeviceAndContextsVk(engineVkCreateInfo, &_renderDevice, &_deviceContext);
        engineFactoryVk->CreateSwapChainVk(_renderDevice, _deviceContext, swapChainDesc, win32NativeWindow, &_swapChain);
#endif
    }

    void RenderingManager::InitializeGraphicsPipeline() {
        GraphicsPipelineStateCreateInfo pipelineStateCreateInfo;
        pipelineStateCreateInfo.PSODesc.Name = "Daedalus Main Pipeline";
        pipelineStateCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        pipelineStateCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        pipelineStateCreateInfo.GraphicsPipeline.RTVFormats[0] = _swapChain->GetDesc().ColorBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.DSVFormat = _swapChain->GetDesc().DepthBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineStateCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        pipelineStateCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
    }

    void RenderingManager::Render() {
        std::tuple<ITextureView*, ITextureView*> renderTargets = SetRenderTargets();
        ClearViews(renderTargets);
    }

    void RenderingManager::Present() {
        _swapChain->Present();
    }

    std::tuple<ITextureView*, ITextureView*> RenderingManager::SetRenderTargets() {
        ITextureView* renderTargetView = _swapChain->GetCurrentBackBufferRTV();
        ITextureView* depthStencilView = _swapChain->GetDepthBufferDSV();

        _deviceContext->SetRenderTargets(1, &renderTargetView, depthStencilView, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        return std::make_tuple(renderTargetView, depthStencilView);
    }

    void RenderingManager::ClearViews(std::tuple<ITextureView *, ITextureView *> renderTargets) {
        const float clearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
        _deviceContext->ClearRenderTarget(std::get<0>(renderTargets), clearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _deviceContext->ClearDepthStencil(std::get<1>(renderTargets), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
} // DaedalusEngine