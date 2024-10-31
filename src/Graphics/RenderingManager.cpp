//
// Created by neoro on 05/05/2024.
//

#include <vector>
#include "../../include/Graphics/RenderingManager.h"
#include "../../include/Graphics/Vertex.h"
#include "../../include/Utilities/Utilities.h"

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
        width = nativeWindowInformation->width;
        height = nativeWindowInformation->height;

        InitializeEngine(nativeWindowInformation);
    }

    RenderingManager::~RenderingManager() {

    }

    void RenderingManager::InitializeEngine(NativeWindowInformation* nativeWindowInformation) {
        bgfx::Init initStruct;
        initStruct.type = bgfx::RendererType::Direct3D12;
        initStruct.vendorId = BGFX_PCI_ID_NVIDIA;
        initStruct.platformData.nwh = nativeWindowInformation->win32Window;
        initStruct.platformData.ndt = nullptr;
        initStruct.platformData.type = bgfx::NativeWindowHandleType::Default;
        initStruct.resolution.width = nativeWindowInformation->width;
        initStruct.resolution.height = nativeWindowInformation->height;

        bgfx::init(initStruct);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setDebug(BGFX_DEBUG_STATS);
    }

    void RenderingManager::InitializeGraphicsPipeline() {

    }

    void RenderingManager::CreateVertexShader(std::string fileName, std::string entryPoint) {

    }

    void RenderingManager::CreateFragmentShader(std::string fileName, std::string entryPoint) {

    }

    void RenderingManager::Render() {
        ClearViews();
    }

    void RenderingManager::Present() {
        bgfx::setViewRect(0, 0, 0, width, height);
        bgfx::touch(0);
        bgfx::frame();
    }

    void RenderingManager::SetRenderTargets() {
        
    }

    void RenderingManager::ClearViews() {
        
    }
} // DaedalusEngine