//
// Created by neoro on 05/05/2024.
//

#include <vector>
#include "RenderingManager.h"
#include "../Utilities/Utilities.h"
#include "Vertex.h"

namespace DaedalusEngine {
    // TODO: Remove these test vertices and indices. This is just test data before model loading is introduced
    std::vector<Vertex> vertices = {
            {glm::vec3 {0.5f, 0.5f, 0.0f}, glm::vec4{1.0f, 0.0f, 0.0f, 0.0f}},
            {glm::vec3{-0.5, 0.5f, 0.0f}, glm::vec4{0.0f, 1.0f, 0.0f, 0.0f}},
            {glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec4{0.0f, 0.0f, 1.0f, 0.0f}},
            {glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}}
    };

    std::vector<int> indices = {
            1, 2,3,
            0, 1, 3
    };

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

        CreateVertexShader("vertex.shader", "main");
        CreateFragmentShader("fragment.shader", "main");
        _cubeBuffer = CreateVertexBuffer("CubeData", vertices);
        _cubeIndexBuffer = CreateIndexBuffer("CubeIndexData", indices);

        LayoutElement layoutElements[] = {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement {1, 0, 4, VT_FLOAT32, False}
        };

        pipelineStateCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = layoutElements;
        pipelineStateCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(layoutElements);
        pipelineStateCreateInfo.pVS = _vertexShader;
        pipelineStateCreateInfo.pPS = _fragmentShader;
        pipelineStateCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        _renderDevice->CreateGraphicsPipelineState(pipelineStateCreateInfo, &_pipelineState);
        _pipelineState->CreateShaderResourceBinding(&_shaderResourceBinding, true);
    }

    void RenderingManager::CreateVertexShader(std::string fileName, std::string entryPoint) {
        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        shaderCreateInfo.Desc.UseCombinedTextureSamplers = true;

        std::string vertexContents = read_shader_file(fileName.c_str());

        shaderCreateInfo.Desc.ShaderType = SHADER_TYPE_VERTEX;
        shaderCreateInfo.EntryPoint = entryPoint.c_str();
        shaderCreateInfo.Desc.Name = "VertexShader";
        shaderCreateInfo.Source = vertexContents.c_str();
        _renderDevice->CreateShader(shaderCreateInfo, &_vertexShader);
    }

    void RenderingManager::CreateFragmentShader(std::string fileName, std::string entryPoint) {
        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        shaderCreateInfo.Desc.UseCombinedTextureSamplers = true;

        std::string fragmentContents = read_shader_file(fileName.c_str());

        shaderCreateInfo.Desc.ShaderType = SHADER_TYPE_PIXEL;
        shaderCreateInfo.EntryPoint = entryPoint.c_str();
        shaderCreateInfo.Desc.Name = "FragmentShader";
        shaderCreateInfo.Source = fragmentContents.c_str();
        _renderDevice->CreateShader(shaderCreateInfo, &_fragmentShader);
    }

    IBuffer *RenderingManager::CreateVertexBuffer(std::string name, std::vector<Vertex>& data) {
        IBuffer* vertexBuffer = nullptr;

        size_t sizeOfData = data.size() * sizeof(data[0]);

        BufferDesc vertexBufferDesc;
        vertexBufferDesc.Name = name.c_str();
        vertexBufferDesc.Size = sizeOfData;
        vertexBufferDesc.Usage = USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = BIND_VERTEX_BUFFER;
        BufferData vertexBufferData;
        vertexBufferData.pData = data.data();
        vertexBufferData.DataSize = sizeOfData;
        _renderDevice->CreateBuffer(vertexBufferDesc, &vertexBufferData, &vertexBuffer);

        return vertexBuffer;
    }

    IBuffer *RenderingManager::CreateIndexBuffer(std::string name, std::vector<int>& data) {
        IBuffer* indexBuffer = nullptr;

        size_t sizeOfData = data.size() * sizeof(data[0]);

        BufferDesc indexBufferDesc;
        indexBufferDesc.Name = name.c_str();
        indexBufferDesc.Usage = USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = BIND_INDEX_BUFFER;
        indexBufferDesc.Size = sizeOfData;
        BufferData indexBufferData;
        indexBufferData.pData = data.data();
        indexBufferData.DataSize = sizeOfData;
        _renderDevice->CreateBuffer(indexBufferDesc, &indexBufferData, &indexBuffer);

        return indexBuffer;
    }

    void RenderingManager::Render() {
        std::tuple<ITextureView*, ITextureView*> renderTargets = SetRenderTargets();
        ClearViews(renderTargets);

        const Uint64 offset = 0;
        IBuffer* buffers[] = {_cubeBuffer};
        _deviceContext->SetVertexBuffers(0, 1, buffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        _deviceContext->SetIndexBuffer(_cubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _deviceContext->SetPipelineState(_pipelineState);
        _deviceContext->CommitShaderResources(_shaderResourceBinding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs drawIndexedAttribs;
        drawIndexedAttribs.IndexType = VT_UINT32;
        drawIndexedAttribs.NumIndices = 6;
        drawIndexedAttribs.Flags = DRAW_FLAG_VERIFY_ALL;
        _deviceContext->DrawIndexed(drawIndexedAttribs);
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