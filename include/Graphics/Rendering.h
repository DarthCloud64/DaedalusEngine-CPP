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
#include <optional>


namespace DaedalusEngine {
    struct Extension {
        const char** extensions;
        uint32_t extensionCount;
    };

    struct Rendering {
        VkInstance vulkanInstance;
        VkPhysicalDevice vulkanPhysicalDevice;
        VkDevice vulkanLogicalDevice;
        VkQueue vulkanGraphicsQueue;
        VkQueue vulkanPresentQueue;
        VkSurfaceKHR vulkanSurface;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
    };

    Rendering* InitializeRenderingEngine(GLFWwindow* glfwWindow);
    VkInstance CreateVulkanInstance();
    VkPhysicalDevice SelectVulkanPhysicalDevice(VkInstance vulkanInstance, VkSurfaceKHR vulkanSurface);
    VkDevice CreateLogicalDevice(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);
    VkQueue GetDeviceQueue(VkDevice vulkanLogicalDevice, uint32_t queueFamilyIndex);
    bool IsPhysicalDeviceSuitable(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);
    Extension GetGlfwRequiredInstanceExtensions();
    std::vector<VkExtensionProperties> GetSupportedVulkanExtensions();
    std::vector<VkExtensionProperties> GetSupportedVulkanDeviceExtensions(VkPhysicalDevice vulkanPhysicalDevice);
    std::vector<VkLayerProperties> GetSupportedVulkanLayers();
    bool ExtensionRequirementsMet(Extension requiredExtensionData, std::vector<VkExtensionProperties> availableExtensions);
    bool ExtensionRequirementsMet(std::vector<std::string> requiredExtensionData, std::vector<VkExtensionProperties> availableExtensions);
    bool LayerRequirementsMet(std::vector<const char*> requiredValidationLayers, std::vector<VkLayerProperties> availableLayers);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);
    VkSurfaceKHR CreateSurface(VkInstance vulkanInstance, GLFWwindow* glfwWindow);
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
