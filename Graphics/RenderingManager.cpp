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
        _vkInstance = CreateVulkanInstance(nativeWindowInformation, _vkInstance);
    }

    RenderingManager::~RenderingManager() {

    }

    void RenderingManager::Render() {
        // std::tuple<ITextureView*, ITextureView*> renderTargets = SetRenderTargets();
        // ClearViews(renderTargets);

        // const Uint64 offset = 0;
        // IBuffer* buffers[] = {_cubeBuffer};
        // _deviceContext->SetVertexBuffers(0, 1, buffers, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        // _deviceContext->SetIndexBuffer(_cubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        // _deviceContext->SetPipelineState(_pipelineState);
        // _deviceContext->CommitShaderResources(_shaderResourceBinding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // DrawIndexedAttribs drawIndexedAttribs;
        // drawIndexedAttribs.IndexType = VT_UINT32;
        // drawIndexedAttribs.NumIndices = 6;
        // drawIndexedAttribs.Flags = DRAW_FLAG_VERIFY_ALL;
        // _deviceContext->DrawIndexed(drawIndexedAttribs);
    }

    void RenderingManager::Present() {
        // _swapChain->Present();
    }

    VkInstance* CreateVulkanInstance(NativeWindowInformation* nativeWindowInformation, VkInstance* vkInstance){
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Daedalus";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "NA";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = nativeWindowInformation->extensionCount;
        createInfo.ppEnabledExtensionNames = nativeWindowInformation->extensions;
        createInfo.enabledLayerCount = 0;

        vkCreateInstance(&createInfo, nullptr, vkInstance);

        return vkInstance;
    }
} // DaedalusEngine