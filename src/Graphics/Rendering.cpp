//
// Created by neoro on 05/05/2024.
//

#include <vector>
#include <string>
#include "../../include/Graphics/Rendering.h"
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

    Rendering* InitializeRenderingEngine(NativeWindowInformation* nativeWindowInformation) {
        printf("Initializing rendering engine...\n");
        Rendering* rendering = new Rendering();

        rendering->vulkanInstance = CreateVulkanInstance();
        rendering->vulkanPhysicalDevice = SelectVulkanPhysicalDevice(rendering->vulkanInstance);

        return rendering;
    }

    VkInstance CreateVulkanInstance() {
        printf("Creating Vulkan instance...\n");
        VkApplicationInfo vkApplicationInfo{};
        vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vkApplicationInfo.pApplicationName = "Daedalus Engine";
        vkApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        vkApplicationInfo.pEngineName = "Daedalus";
        vkApplicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
        vkApplicationInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo vkInstanceCreateInfo{};
        vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;

        Extension glfwExtensionData = GetGlfwRequiredInstanceExtensions();
        std::vector<VkExtensionProperties> supportedExtensions = GetSupportedVulkanExtensions();
        printf("Checking Vulkan instance contains required extensions for GLFW...\n");
        ExtensionRequirementsMet(glfwExtensionData, supportedExtensions);
        const std::vector<const char*> requiredValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        std::vector<VkLayerProperties> supportedValidationLayers = GetSupportedVulkanLayers();
        LayerRequirementsMet(requiredValidationLayers, supportedValidationLayers);

        vkInstanceCreateInfo.enabledExtensionCount = glfwExtensionData.extensionCount;
        vkInstanceCreateInfo.ppEnabledExtensionNames = glfwExtensionData.extensions;
        vkInstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
        vkInstanceCreateInfo.ppEnabledLayerNames = requiredValidationLayers.data();

        VkInstance vkInstance;
        if (vkCreateInstance(&vkInstanceCreateInfo, nullptr, &vkInstance) != VK_SUCCESS) {
            printf("Failed to create Vulkan instance!\n");
        }

        return vkInstance;
    }

    bool ExtensionRequirementsMet(Extension requiredExtensionData, std::vector<VkExtensionProperties> availableExtensions) {
        bool oneOrMoreRequiredExtensionsNotFound = false;

        for (int i = 0; i < requiredExtensionData.extensionCount; i++) {
            bool requiredExtensionIsAvailable = false;

            for (const auto& availableExtension : availableExtensions) {
                if (strcmp(requiredExtensionData.extensions[i], availableExtension.extensionName) == 0) {
                    requiredExtensionIsAvailable = true;
                }
            }

            if (!requiredExtensionIsAvailable) {
                oneOrMoreRequiredExtensionsNotFound = true;
                printf("%s is not available!!\n", requiredExtensionData.extensions[i]);
            }
        }

        return oneOrMoreRequiredExtensionsNotFound;
    }

    bool LayerRequirementsMet(std::vector<const char*> requiredValidationLayers, std::vector<VkLayerProperties> availableLayers) {
        bool oneOrMoreRequiredLayersNotFound = false;

        for(const auto& requiredValidationLayer : requiredValidationLayers){
            bool requiredLayerIsAvailable = false;

            for (const auto& availableLayer : availableLayers) {
                if (strcmp(requiredValidationLayer, availableLayer.layerName) == 0) {
                    requiredLayerIsAvailable = true;
                }
            }

            if (!requiredLayerIsAvailable) {
                oneOrMoreRequiredLayersNotFound = true;
                printf("%s is not available!!\n", requiredValidationLayer);
            }
        }

        return oneOrMoreRequiredLayersNotFound;
    }

    std::vector<VkLayerProperties> GetSupportedVulkanLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> supportedLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());

        printf("Supported layers:\n");

        for (const auto& layer : supportedLayers) {
            printf("%s\n", layer.layerName);
        }

        return supportedLayers;
    }

    std::vector<VkExtensionProperties> GetSupportedVulkanExtensions() {
        printf("Retrieve supported list of Vulkan extensions...\n");

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());

        printf("Supported extensions:\n");

        for (const auto& extension : supportedExtensions) {
            printf("%s\n", extension.extensionName);
        }

        return supportedExtensions;
    }

    Extension GetGlfwRequiredInstanceExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        Extension extensionData = Extension{};
        extensionData.extensions = glfwExtensions;
        extensionData.extensionCount = glfwExtensionCount;

        return extensionData;
    }

    VkPhysicalDevice SelectVulkanPhysicalDevice(VkInstance vulkanInstance) {
        VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());

        for (const auto& physicalDevice : physicalDevices) {
            if (IsPhysicalDeviceSuitable(physicalDevice)) {
                selectedPhysicalDevice = physicalDevice;
            }
        }

        if (selectedPhysicalDevice == VK_NULL_HANDLE) {
            printf("Failed to find capable GPU!\n");
        }

        return selectedPhysicalDevice;
    }

    bool IsPhysicalDeviceSuitable(VkPhysicalDevice vulkanPhysicalDevice) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        vkGetPhysicalDeviceProperties(vulkanPhysicalDevice, &physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(vulkanPhysicalDevice, &physicalDeviceFeatures);

        if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physicalDeviceFeatures.geometryShader) {
            printf("GPU '%s' meets requirements\n", physicalDeviceProperties.deviceName);
            return true;
        }

        return false;
    }

    void InitializeGraphicsPipeline() {

    }

    void CreateVertexShader(std::string fileName, std::string entryPoint) {

    }

    void CreateFragmentShader(std::string fileName, std::string entryPoint) {

    }

    void Render() {

    }

    void Present() {

    }

    void SetRenderTargets() {
        
    }

    void ClearViews() {
        
    }

    void CleanupRendering(Rendering* rendering) {
        vkDestroyInstance(rendering->vulkanInstance, nullptr);
    }
} // DaedalusEngine