//
// Created by neoro on 05/05/2024.
//

#include <vector>
#include <string>
#include <set>
#include <algorithm>
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

    Rendering* InitializeRenderingEngine(GLFWwindow* glfwWindow) {
        printf("Initializing rendering engine...\n");
        Rendering* rendering = new Rendering();

        rendering->vulkanInstance = CreateVulkanInstance();
        rendering->vulkanSurface = CreateSurface(rendering->vulkanInstance, glfwWindow);
        rendering->vulkanPhysicalDevice = SelectVulkanPhysicalDevice(rendering->vulkanInstance, rendering->vulkanSurface);
        rendering->vulkanLogicalDevice = CreateLogicalDevice(rendering->vulkanPhysicalDevice, rendering->vulkanSurface);
        rendering->vulkanGraphicsQueue = GetDeviceQueue(rendering->vulkanLogicalDevice, FindQueueFamilies(rendering->vulkanPhysicalDevice, rendering->vulkanSurface).graphicsFamily.value());
        rendering->vulkanPresentQueue = GetDeviceQueue(rendering->vulkanLogicalDevice, FindQueueFamilies(rendering->vulkanPhysicalDevice, rendering->vulkanSurface).presentFamily.value());
        rendering->vulkanSwapChain = CreateSwapChain(rendering->vulkanPhysicalDevice, rendering->vulkanLogicalDevice, rendering->vulkanSurface, glfwWindow);
        rendering->swapChainImages = GetSwapChainImages(rendering->vulkanLogicalDevice, rendering->vulkanSwapChain);

        SwapChainSupportDetails swapChainSupportDetails = GetSwapChainSupport(rendering->vulkanPhysicalDevice, rendering->vulkanSurface);

        rendering->selectedFormat = SelectSwapSurfaceFormat(swapChainSupportDetails.formats).format;
        rendering->selectedExtent = SelectSwapExtent(swapChainSupportDetails.capabilities, glfwWindow);
        rendering->swapChainImageViews = CreateImageViews(rendering->swapChainImages, rendering->selectedFormat, rendering->vulkanLogicalDevice);
        rendering->vulkanGraphicsPipeline = CreateGraphicsPipeline();

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

    bool ExtensionRequirementsMet(const std::vector<std::string>& requiredExtensionData, const std::vector<VkExtensionProperties>& availableExtensions) {
        bool oneOrMoreRequiredExtensionsNotFound = false;

        for (const auto& extension : requiredExtensionData) {
            bool requiredExtensionIsAvailable = false;

            for (const auto& availableExtension : availableExtensions) {
                if (strcmp(extension.c_str(), availableExtension.extensionName) == 0) {
                    requiredExtensionIsAvailable = true;
                }
            }

            if (!requiredExtensionIsAvailable) {
                oneOrMoreRequiredExtensionsNotFound = true;
                printf("%s is not available!!\n", extension);
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

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface) {
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

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vulkanPhysicalDevice, counter, vulkanSurface, &presentSupport);

            if (presentSupport) {
                queueFamilyIndices.presentFamily = counter;
            }

            counter++;
        }

        return queueFamilyIndices;
    }

    VkSurfaceKHR CreateSurface(VkInstance vulkanInstance, GLFWwindow* glfwWindow) {
        VkSurfaceKHR vulkanSurface;

        if (glfwCreateWindowSurface(vulkanInstance, glfwWindow, nullptr, &vulkanSurface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create vulkan surface!\n");
        }

        return vulkanSurface;
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

    SwapChainSupportDetails GetSwapChainSupport(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface) {
        SwapChainSupportDetails swapChainSupportDetails;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanPhysicalDevice, vulkanSurface, &swapChainSupportDetails.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice, vulkanSurface, &formatCount, nullptr);
        if (formatCount != 0) {
            swapChainSupportDetails.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice, vulkanSurface, &formatCount, swapChainSupportDetails.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice, vulkanSurface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            swapChainSupportDetails.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice, vulkanSurface, &presentModeCount, swapChainSupportDetails.presentModes.data());
        }

        return swapChainSupportDetails;
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

    std::vector<VkExtensionProperties> GetSupportedVulkanDeviceExtensions(VkPhysicalDevice vulkanPhysicalDevice) {
        printf("Retrieve supported list of Vulkan device extensions...\n");

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(vulkanPhysicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(vulkanPhysicalDevice, nullptr, &extensionCount, supportedExtensions.data());

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

    VkPhysicalDevice SelectVulkanPhysicalDevice(VkInstance vulkanInstance, VkSurfaceKHR vulkanSurface) {
        VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());

        for (const auto& physicalDevice : physicalDevices) {
            if (IsPhysicalDeviceSuitable(physicalDevice, vulkanSurface)) {
                selectedPhysicalDevice = physicalDevice;
            }
        }

        if (selectedPhysicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find capable GPU!\n");
        }

        return selectedPhysicalDevice;
    }

    VkDevice CreateLogicalDevice(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface) {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vulkanPhysicalDevice, vulkanSurface);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };

        float priority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
            deviceQueueCreateInfo.queueCount = 1;
            deviceQueueCreateInfo.pQueuePriorities = &priority;

            queueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        VkPhysicalDeviceFeatures physicalDeviceFeatures{};

        std::vector<const char*> requiredDeviceExtensions;
        requiredDeviceExtensions.push_back("VK_KHR_swapchain");

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
        deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());

        VkDevice vulkanLogicalDevice;
        if (vkCreateDevice(vulkanPhysicalDevice, &deviceCreateInfo, nullptr, &vulkanLogicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!\n");
        }

        return vulkanLogicalDevice;
    }

    bool IsPhysicalDeviceSuitable(VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurface) {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        vkGetPhysicalDeviceProperties(vulkanPhysicalDevice, &physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(vulkanPhysicalDevice, &physicalDeviceFeatures);
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vulkanPhysicalDevice, vulkanSurface);
        
        std::vector<std::string> requiredDeviceExtensions;
        requiredDeviceExtensions.push_back("VK_KHR_swapchain");
        std::vector<VkExtensionProperties> supportedExtensions = GetSupportedVulkanDeviceExtensions(vulkanPhysicalDevice);

        SwapChainSupportDetails swapChainSupportDetails;
        if (ExtensionRequirementsMet(requiredDeviceExtensions, supportedExtensions)) {
            swapChainSupportDetails = GetSwapChainSupport(vulkanPhysicalDevice, vulkanSurface);
        }

        if (
            physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
            && physicalDeviceFeatures.geometryShader
            && queueFamilyIndices.graphicsFamily.has_value()
            && queueFamilyIndices.presentFamily.has_value()
            && ExtensionRequirementsMet(requiredDeviceExtensions, supportedExtensions)
            && !swapChainSupportDetails.formats.empty()
            && !swapChainSupportDetails.presentModes.empty()) {
            printf("GPU '%s' meets requirements\n", physicalDeviceProperties.deviceName);
            return true;
        }

        return false;
    }

    VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (
                availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
                && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR capabilities, GLFWwindow* glfwWindow) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            int width;
            int height;
            glfwGetFramebufferSize(glfwWindow, &width, &height);
            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    VkSwapchainKHR CreateSwapChain(VkPhysicalDevice vulkanPhysicalDevice, VkDevice vulkanLogicalDevice, VkSurfaceKHR vulkanSurface, GLFWwindow* glfwWindow) {
        SwapChainSupportDetails swapChainSupportDetails = GetSwapChainSupport(vulkanPhysicalDevice, vulkanSurface);

        VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(swapChainSupportDetails.formats);
        VkPresentModeKHR presentMode = SelectSwapPresentMode(swapChainSupportDetails.presentModes);
        VkExtent2D extent = SelectSwapExtent(swapChainSupportDetails.capabilities, glfwWindow);

        uint32_t swapChainImageCount = swapChainSupportDetails.capabilities.minImageCount + 1;
        if (swapChainSupportDetails.capabilities.maxImageCount > 0 && swapChainImageCount > swapChainSupportDetails.capabilities.maxImageCount) {
            swapChainImageCount = swapChainSupportDetails.capabilities.maxImageCount;
        }

        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vulkanPhysicalDevice, vulkanSurface);

        VkSwapchainCreateInfoKHR swapChainCreateInfo{};
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = vulkanSurface;
        swapChainCreateInfo.minImageCount = swapChainImageCount;
        swapChainCreateInfo.imageFormat = surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapChainCreateInfo.imageExtent = extent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndicesArr[] = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };
        if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = 2;
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndicesArr;
        }
        else {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0;
            swapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapChainCreateInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.presentMode = presentMode;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        VkSwapchainKHR swapChain;
        if (vkCreateSwapchainKHR(vulkanLogicalDevice, &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("Error creating swap chain!\n");
        }

        return swapChain;
    }

    std::vector<VkImage> GetSwapChainImages(VkDevice vulkanLogicalDevice, VkSwapchainKHR vulkanSwapChain) {
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(vulkanLogicalDevice, vulkanSwapChain, &imageCount, nullptr);
        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(vulkanLogicalDevice, vulkanSwapChain, &imageCount, images.data());

        return images;
    }

    std::vector<VkImageView> CreateImageViews(const std::vector<VkImage>& swapChainImages, VkFormat selectedFormat, VkDevice logicalDevice) {
        std::vector<VkImageView> swapChainImageViews(swapChainImages.size());

        for (int i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo imageViewCreateInfo{};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.image = swapChainImages[i];
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = selectedFormat;
            imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
            imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
            imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
            imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;


            if (vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create an image view!\n");
            }
        }

        return swapChainImageViews;
    }

    VkPipeline CreateGraphicsPipeline(VkDevice logicalDevice) {
        std::vector<char> vertexShaderCode = ReadFileAsBinary("C:\\Code\\cpp\\DaedalusEngine-CPP\\shaders\\vert.spv");
        std::vector<char> fragmentShaderCode = ReadFileAsBinary("C:\\Code\\cpp\\DaedalusEngine-CPP\\shaders\\frag.spv");
        VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode, logicalDevice);
        VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode, logicalDevice);

        VkPipelineShaderStageCreateInfo vertexStageCreateInfo{};
        vertexStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexStageCreateInfo.module = vertexShaderModule;
        vertexStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentStageCreateInfo{};
        fragmentStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentStageCreateInfo.module = fragmentShaderModule;
        fragmentStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStagesCreateInfos[] = { vertexStageCreateInfo, fragmentStageCreateInfo };

        vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
        return nullptr;
    }

    VkShaderModule CreateShaderModule(const std::vector<char>& rawBinaryShaderCode, VkDevice logicalDevice) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = rawBinaryShaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(rawBinaryShaderCode.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a shader module!\n");
        }

        return shaderModule;
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
        for (auto imageView : rendering->swapChainImageViews) {
            vkDestroyImageView(rendering->vulkanLogicalDevice, imageView, nullptr);
        }

        vkDestroySwapchainKHR(rendering->vulkanLogicalDevice, rendering->vulkanSwapChain, nullptr);
        vkDestroyDevice(rendering->vulkanLogicalDevice, nullptr);
        vkDestroySurfaceKHR(rendering->vulkanInstance, rendering->vulkanSurface, nullptr);
        vkDestroyInstance(rendering->vulkanInstance, nullptr);
    }
} // DaedalusEngine