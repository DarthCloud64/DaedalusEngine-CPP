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
#include "../../external/VkBootstrap.h"
#include <SDL2/SDL_vulkan.h>

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

    Rendering* InitializeRenderingEngine(SDL_Window* abstractedWindow) {
        printf("Initializing rendering engine...\n");
        Rendering* rendering = new Rendering();

        rendering = InitializeCoreVulkanComponents(rendering, abstractedWindow);

        // rendering->vulkanGraphicsPipeline = CreateGraphicsPipeline();

        return rendering;
    }

    Rendering* InitializeCoreVulkanComponents(Rendering* rendering, SDL_Window* abstractedWindow){
        vkb::InstanceBuilder instanceBuilder;

        // Build the vulkan instance
        vkb::Result<vkb::Instance> vkbInstanceResult = instanceBuilder.set_app_name("Daedalus")
            .request_validation_layers(true)
            .use_default_debug_messenger()
            .require_api_version(1, 1, 0)
            .build();

        rendering->vulkanInstance = vkbInstanceResult.value().instance;
        rendering->vulkanMessenger = vkbInstanceResult.value().debug_messenger;

        // Create the vulkan drawing surface on the SDL window
        SDL_Vulkan_CreateSurface(abstractedWindow, rendering->vulkanInstance, &rendering->vulkanSurface);

        // Select the physical device (GPU)
        vkb::PhysicalDeviceSelector physicalDeviceSelector {vkbInstanceResult.value()};
        vkb::PhysicalDevice vkbPhysicalDevice = physicalDeviceSelector
            .set_minimum_version(1, 1)
            .set_surface(rendering->vulkanSurface)
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .select()
            .value();

        // Build the vulkan logical device
        vkb::DeviceBuilder deviceBuilder {vkbPhysicalDevice};
        vkb::Device vkbDevice = deviceBuilder.build().value();

        rendering->vulkanPhysicalDevice = vkbPhysicalDevice.physical_device;
        rendering->vulkanLogicalDevice = vkbDevice.device;
        rendering->vulkanGraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        rendering->graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

        int width;
        int height;
        SDL_GetWindowSize(abstractedWindow, &width, &height);

        // Build the swap chain which vulkan will render into
        // The swap chain images are flipped to the surface/window
        vkb::SwapchainBuilder swapChainBuilder {vkbPhysicalDevice, vkbDevice, rendering->vulkanSurface};
        vkb::Swapchain vkbSwapchain = swapChainBuilder
            .use_default_format_selection()
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .set_desired_extent(width, height)
            .build()
            .value();

        rendering->vulkanSwapChain = vkbSwapchain.swapchain;
        rendering->swapChainImages = vkbSwapchain.get_images().value();
        rendering->swapChainImageViews = vkbSwapchain.get_image_views().value();

        return rendering;
    }

    Rendering* InitializeVulkanCommands(Rendering* rendering){
        // Create a command pool which will allocate a command buffer to store commands
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = nullptr;

        // Pool will create commands related to the graphics family
        commandPoolCreateInfo.queueFamilyIndex = rendering->graphicsQueueFamily;

        // This flag indicates we want the ability to reset command buffers
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if(vkCreateCommandPool(rendering->vulkanLogicalDevice, &commandPoolCreateInfo, nullptr, &rendering->vulkanCommandPool) != VK_SUCCESS){
            throw std::runtime_error("Failed to create the command pool!\n");
        }

        // Allocate a command buffer
        VkCommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.commandPool = rendering->vulkanCommandPool;
        allocateInfo.commandBufferCount = 1;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        if(vkAllocateCommandBuffers(rendering->vulkanLogicalDevice, &allocateInfo, &rendering->vulkanCommandBuffer) != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate the command buffer!\n");
        }

        return rendering;
    }

    Rendering* InitializeDefaultRenderPass(Rendering* rendering){

    }

    Rendering* InitializeFramebuffers(Rendering* rendering){

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
        vkDestroyCommandPool(rendering->vulkanLogicalDevice, rendering->vulkanCommandPool, nullptr);

        for (auto imageView : rendering->swapChainImageViews) {
            vkDestroyImageView(rendering->vulkanLogicalDevice, imageView, nullptr);
        }

        vkDestroySwapchainKHR(rendering->vulkanLogicalDevice, rendering->vulkanSwapChain, nullptr);
        vkDestroyDevice(rendering->vulkanLogicalDevice, nullptr);
        vkDestroySurfaceKHR(rendering->vulkanInstance, rendering->vulkanSurface, nullptr);
        vkb::destroy_debug_utils_messenger(rendering->vulkanInstance, rendering->vulkanMessenger);
        vkDestroyInstance(rendering->vulkanInstance, nullptr);
    }
} // DaedalusEngine