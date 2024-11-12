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
        VkSwapchainKHR vulkanSwapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat selectedFormat;
        VkExtent2D selectedExtent;
        std::vector<VkImageView> swapChainImageViews;
        VkPipeline vulkanGraphicsPipeline;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
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
    SwapChainSupportDetails GetSwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);
    bool ExtensionRequirementsMet(Extension requiredExtensionData, std::vector<VkExtensionProperties> availableExtensions);
    bool ExtensionRequirementsMet(const std::vector<std::string>& requiredExtensionData, const std::vector<VkExtensionProperties>& availableExtensions);
    bool LayerRequirementsMet(std::vector<const char*> requiredValidationLayers, std::vector<VkLayerProperties> availableLayers);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface);
    VkSurfaceKHR CreateSurface(VkInstance vulkanInstance, GLFWwindow* glfwWindow);
    VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow* glfwWindow);
    VkSwapchainKHR CreateSwapChain(VkPhysicalDevice vulkanPhysicalDevice, VkDevice vulkanLogicalDevice, VkSurfaceKHR vulkanSurface, GLFWwindow* glfwWindow);
    std::vector<VkImage> GetSwapChainImages(VkDevice vulkanLogicalDevice, VkSwapchainKHR vulkanSwapChain);
    std::vector<VkImageView> CreateImageViews(const std::vector<VkImage>& swapChainImages, VkFormat selectedFormat, VkDevice logicalDevice);
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
