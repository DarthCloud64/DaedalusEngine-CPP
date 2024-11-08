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
#include "../../include/Window/NativeWindowInformation.h"

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace DaedalusEngine {
    struct Extension {
        const char** extensions;
        uint32_t extensionCount;
    };

    struct Rendering {
        VkInstance vulkanInstance;
        VkPhysicalDevice vulkanPhysicalDevice;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily;
    };

    Rendering* InitializeRenderingEngine(NativeWindowInformation* nativeWindowInformation);
    VkInstance CreateVulkanInstance();
    VkPhysicalDevice SelectVulkanPhysicalDevice(VkInstance vulkanInstance);
    bool IsPhysicalDeviceSuitable(VkPhysicalDevice vulkanPhysicalDevice);
    Extension GetGlfwRequiredInstanceExtensions();
    std::vector<VkExtensionProperties> GetSupportedVulkanExtensions();
    std::vector<VkLayerProperties> GetSupportedVulkanLayers();
    bool ExtensionRequirementsMet(Extension requiredExtensionData, std::vector<VkExtensionProperties> availableExtensions);
    bool LayerRequirementsMet(std::vector<const char*> requiredValidationLayers, std::vector<VkLayerProperties> availableLayers);
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
