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

        vkInstanceCreateInfo.enabledExtensionCount = glfwExtensionData.extensionCount;
        vkInstanceCreateInfo.ppEnabledExtensionNames = glfwExtensionData.extensions;
        vkInstanceCreateInfo.enabledLayerCount = 0;

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