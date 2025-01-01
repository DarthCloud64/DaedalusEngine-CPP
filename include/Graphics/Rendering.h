//
// Created by neoro on 05/05/2024.
//

#ifndef DAEDALUSENGINE_RENDERINGMANAGER_H
#define DAEDALUSENGINE_RENDERINGMANAGER_H

#include <memory>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <optional>


namespace DaedalusEngine {
    struct Extension {
        const char** extensions;
        uint32_t extensionCount;
    };

    struct Rendering {
        VkInstance vulkanInstance;
        VkDebugUtilsMessengerEXT vulkanMessenger;
        VkPhysicalDevice vulkanPhysicalDevice;
        VkDevice vulkanLogicalDevice;
        VkQueue vulkanGraphicsQueue;
        uint32_t graphicsQueueFamily;
        VkCommandPool vulkanCommandPool;
        VkCommandBuffer vulkanCommandBuffer;
        VkRenderPass vulkanRenderPass;
        std::vector<VkFramebuffer> frameBuffers;
        VkQueue vulkanPresentQueue;
        VkSurfaceKHR vulkanSurface;
        VkSwapchainKHR vulkanSwapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkPipeline vulkanGraphicsPipeline;
    };

    Rendering* InitializeRenderingEngine(SDL_Window* abstractedWindow);
    Rendering* InitializeCoreVulkanComponents(Rendering* rendering, SDL_Window* abstractedWindow);
    Rendering* InitializeVulkanCommands(Rendering* rendering);
    Rendering* InitializeDefaultRenderPass(Rendering* rendering);
    Rendering* InitializeFramebuffers(Rendering* rendering);
    VkPipeline CreateGraphicsPipeline(VkDevice logicalDevice);
    VkShaderModule CreateShaderModule(const std::vector<char>& rawBinaryShaderCode, VkDevice logicalDevice);
    void CleanupRendering(Rendering* rendering);
    void SetRenderTargets();
    void ClearViews();
    void CreateVertexShader(std::string fileName, std::string entryPoint);
    void CreateFragmentShader(std::string fileName, std::string entryPoint);
    void InitializeGraphicsPipeline();
    void Render();
    void Present();
} // DaedalusEngine/

#endif //DAEDALUSENGINE_RENDERINGMANAGER_H
