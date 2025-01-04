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
    // Defines double-buffering. 
    // While the GPU is busy executing commands for a frame, the CPU can proceed with the next frame
    const unsigned int FRAME_OVERLAP = 2;

    struct Extension {
        const char** extensions;
        uint32_t extensionCount;
    };

    struct FrameData {
        VkFence renderFence;

        // Semaphore used to control presenting the image to the OS once drawing finishes
        VkSemaphore renderSemaphore;

        // Semaphore used so that the render commands wait on the swapchain image request
        VkSemaphore swapchainSemaphore;

        VkCommandPool vulkanCommandPool;
        VkCommandBuffer vulkanCommandBuffer;
    };

    struct Rendering {
        int frameNumber{0};
        VkInstance vulkanInstance;
        VkDebugUtilsMessengerEXT vulkanMessenger;
        VkPhysicalDevice vulkanPhysicalDevice;
        VkDevice vulkanLogicalDevice;
        VkQueue vulkanGraphicsQueue;
        uint32_t graphicsQueueFamily;
        std::vector<VkFramebuffer> frameBuffers;
        VkQueue vulkanPresentQueue;
        VkSurfaceKHR vulkanSurface;
        VkSwapchainKHR vulkanSwapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkPipeline vulkanGraphicsPipeline;
        VkExtent2D vulkanExtent;
        FrameData frameData[FRAME_OVERLAP];
    };

    Rendering* InitializeRenderingEngine(SDL_Window* abstractedWindow);
    Rendering* InitializeCoreVulkanComponents(Rendering* rendering, SDL_Window* abstractedWindow);
    Rendering* InitializeVulkanCommands(Rendering* rendering);
    Rendering* InitializeVulkanSyncStructures(Rendering* rendering);
    VkPipeline CreateGraphicsPipeline(VkDevice logicalDevice);
    VkShaderModule CreateShaderModule(const std::vector<char>& rawBinaryShaderCode, VkDevice logicalDevice);
    FrameData& GetCurrentFrame(Rendering* rendering);
    void TransitionImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
    void CleanupRendering(Rendering* rendering);
    void SetRenderTargets();
    void ClearViews();
    void CreateVertexShader(std::string fileName, std::string entryPoint);
    void CreateFragmentShader(std::string fileName, std::string entryPoint);
    void InitializeGraphicsPipeline();
    void Draw(Rendering* rendering);
    void Present();
} // DaedalusEngine/

#endif //DAEDALUSENGINE_RENDERINGMANAGER_H
