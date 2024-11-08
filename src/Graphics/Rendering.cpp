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
        rendering->vulkanLogicalDevice = CreateLogicalDevice(rendering->vulkanPhysicalDevice);
        rendering->vulkanGraphicsQueue = GetDeviceQueue(rendering->vulkanLogicalDevice, FindQueueFamilies(rendering->vulkanPhysicalDevice).graphicsFamily.value());

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
        bool extensionRequirementsMet = ExtensionRequirementsMet(glfwExtensionData, supportedExtensions);
        if (!extensionRequirementsMet){
            throw std::runtime_error("One or more required extensions were not available!\n");
        }

        printf("Checking Vulkan instance contains required layers...\n");
        const std::vector<const char*> requiredValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        std::vector<VkLayerProperties> supportedValidationLayers = GetSupportedVulkanLayers();
        bool layerRequirementsMet = LayerRequirementsMet(requiredValidationLayers, supportedValidationLayers);
        if (!layerRequirementsMet){
            throw std::runtime_error("One or more required layers were not available!\n");
        }

        vkInstanceCreateInfo.enabledExtensionCount = glfwExtensionData.extensionCount;
        vkInstanceCreateInfo.ppEnabledExtensionNames = glfwExtensionData.extensions;
        vkInstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
        vkInstanceCreateInfo.ppEnabledLayerNames = requiredValidationLayers.data();

        VkInstance vkInstance;
        if (vkCreateInstance(&vkInstanceCreateInfo, nullptr, &vkInstance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!\n");
        }

        return vkInstance;
    }

    VkQueue GetDeviceQueue(VkDevice vulkanLogicalDevice, uint32_t queueFamilyIndex) {
        VkQueue retrievedQueue;
        vkGetDeviceQueue(vulkanLogicalDevice, queueFamilyIndex, 0, &retrievedQueue);

        return retrievedQueue;
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

        return !oneOrMoreRequiredExtensionsNotFound;
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

        return !oneOrMoreRequiredLayersNotFound;
    }

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice) {
        QueueFamilyIndices queueFamilyIndices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanPhysicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vulkanPhysicalDevice, &queueFamilyCount, queueFamilies.data());

        int counter = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndices.graphicsFamily = counter;
            }

            counter++;
        }

        return queueFamilyIndices;
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
            throw std::runtime_error("Failed to find capable GPU!\n");
        }

        return selectedPhysicalDevice;
    }

    VkDevice CreateLogicalDevice(VkPhysicalDevice vulkanPhysicalDevice) {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vulkanPhysicalDevice);

        VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        deviceQueueCreateInfo.queueCount = 1;

        float priority = 1.0f;
        deviceQueueCreateInfo.pQueuePriorities = &priority;

        VkPhysicalDeviceFeatures physicalDeviceFeatures{};

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        VkDevice vulkanLogicalDevice;
        if (vkCreateDevice(vulkanPhysicalDevice, &deviceCreateInfo, nullptr, &vulkanLogicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!\n");
        }

        return vulkanLogicalDevice;
    }

    bool IsPhysicalDeviceSuitable(VkPhysicalDevice vulkanPhysicalDevice) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        vkGetPhysicalDeviceProperties(vulkanPhysicalDevice, &physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(vulkanPhysicalDevice, &physicalDeviceFeatures);
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vulkanPhysicalDevice);

        if (
            physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
            && physicalDeviceFeatures.geometryShader
            && queueFamilyIndices.graphicsFamily.has_value()) {
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
        vkDestroyDevice(rendering->vulkanLogicalDevice, nullptr);
        vkDestroyInstance(rendering->vulkanInstance, nullptr);
    }
} // DaedalusEngine