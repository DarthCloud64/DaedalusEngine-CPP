//
// Created by neoro on 05/05/2024.
//

#include <fstream>
#include <sstream>
#include "RenderingManager.h"

namespace DaedalusEngine {
    // TODO: Remove this. This is just test data before model loading is introduced
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    std::string read_shader_file (const char *shader_file)
    {
        std::ifstream file (shader_file);
        if (!file) return std::string ();

        file.ignore(std::numeric_limits<std::streamsize>::max());
        auto size = file.gcount();

        if (size > 0x10000) // 64KiB sanity check for shaders:
            return std::string ();

        file.clear();
        file.seekg(0, std::ios_base::beg);

        std::stringstream sstr;
        sstr << file.rdbuf();
        file.close();

        return sstr.str();
    }

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

        _engineFactoryVk = GetEngineFactoryVk();
        _engineFactoryVk->CreateDeviceAndContextsVk(engineVkCreateInfo, &_renderDevice, &_deviceContext);
        _engineFactoryVk->CreateSwapChainVk(_renderDevice, _deviceContext, swapChainDesc, win32NativeWindow, &_swapChain);
#endif

        InitializeGraphicsPipeline();
    }

    void RenderingManager::InitializeGraphicsPipeline() {
        GraphicsPipelineStateCreateInfo pipelineStateCreateInfo;
        pipelineStateCreateInfo.PSODesc.Name = "Daedalus Main Pipeline";
        pipelineStateCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        pipelineStateCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        pipelineStateCreateInfo.GraphicsPipeline.RTVFormats[0] = _swapChain->GetDesc().ColorBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.DSVFormat = _swapChain->GetDesc().DepthBufferFormat;
        pipelineStateCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineStateCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_FRONT;
        pipelineStateCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        shaderCreateInfo.Desc.UseCombinedTextureSamplers = true;

        std::string vertexContents = read_shader_file("vertex.shader");

        shaderCreateInfo.Desc.ShaderType = SHADER_TYPE_VERTEX;
        shaderCreateInfo.EntryPoint = "main";
        shaderCreateInfo.Desc.Name = "VertexShader";
        shaderCreateInfo.Source = vertexContents.c_str();
        _renderDevice->CreateShader(shaderCreateInfo, &_vertexShader);

        BufferDesc bufferDesc;
        bufferDesc.Name = "TriangleData";
        bufferDesc.Size = sizeof(vertices);
        bufferDesc.Usage = USAGE_IMMUTABLE;
        bufferDesc.BindFlags = BIND_VERTEX_BUFFER;
        BufferData bufferData;
        bufferData.pData = vertices;
        bufferData.DataSize = sizeof(vertices);
        _renderDevice->CreateBuffer(bufferDesc, &bufferData, &_triangleBuffer);

        std::string fragmentContents = read_shader_file("fragment.shader");

        shaderCreateInfo.Desc.ShaderType = SHADER_TYPE_PIXEL;
        shaderCreateInfo.EntryPoint = "main";
        shaderCreateInfo.Desc.Name = "FragmentShader";
        shaderCreateInfo.Source = fragmentContents.c_str();
        _renderDevice->CreateShader(shaderCreateInfo, &_fragmentShader);

        LayoutElement layoutElements[] = {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}
        };

        pipelineStateCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElements;
        pipelineStateCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElements);
        pipelineStateCreateInfo.pVS = _vertexShader;
        pipelineStateCreateInfo.pPS = _fragmentShader;
        pipelineStateCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        _renderDevice->CreateGraphicsPipelineState(pipelineStateCreateInfo, &_pipelineState);
        _pipelineState->CreateShaderResourceBinding(&_shaderResourceBinding, true);
    }

    void RenderingManager::Render() {
        std::tuple<ITextureView*, ITextureView*> renderTargets = SetRenderTargets();
        ClearViews(renderTargets);

        const Uint64 offset = 0;
        IBuffer* buffers[] = {_triangleBuffer};
        _deviceContext->SetVertexBuffers(0, 1, buffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        _deviceContext->SetPipelineState(_pipelineState);
        _deviceContext->CommitShaderResources(_shaderResourceBinding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawAttribs drawAttribs;
        drawAttribs.NumVertices = 3;
        _deviceContext->Draw(drawAttribs);
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