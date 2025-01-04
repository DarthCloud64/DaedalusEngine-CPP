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
#include <math.h>

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
        rendering = InitializeVulkanCommands(rendering);
        rendering = InitializeVulkanSyncStructures(rendering);

        // rendering->vulkanGraphicsPipeline = CreateGraphicsPipeline();

        return rendering;
    }

    Rendering* InitializeCoreVulkanComponents(Rendering* rendering, SDL_Window* abstractedWindow){
        vkb::InstanceBuilder instanceBuilder;

        // Build the vulkan instance
        vkb::Result<vkb::Instance> vkbInstanceResult = instanceBuilder.set_app_name("Daedalus")
            .request_validation_layers(true)
            .use_default_debug_messenger()
            .require_api_version(1, 3, 0)
            .build();

        rendering->vulkanInstance = vkbInstanceResult.value().instance;
        rendering->vulkanMessenger = vkbInstanceResult.value().debug_messenger;

        // Create the vulkan drawing surface on the SDL window
        SDL_Vulkan_CreateSurface(abstractedWindow, rendering->vulkanInstance, &rendering->vulkanSurface);

        //vulkan 1.3 features
        VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
        features.dynamicRendering = true;
        features.synchronization2 = true;

        //vulkan 1.2 features
        VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        features12.bufferDeviceAddress = true;
        features12.descriptorIndexing = true;

        // Select the physical device (GPU)
        vkb::PhysicalDeviceSelector physicalDeviceSelector {vkbInstanceResult.value()};
        vkb::PhysicalDevice vkbPhysicalDevice = physicalDeviceSelector
            .set_minimum_version(1, 3)
            .set_surface(rendering->vulkanSurface)
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .set_required_features_13(features)
            .set_required_features_12(features12)
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
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build()
            .value();

        rendering->vulkanExtent = vkbSwapchain.extent;
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

        // For each entry of the FrameData array, Create a command pool and allocate a buffer
        for(int i = 0; i < FRAME_OVERLAP; i++){
            if(vkCreateCommandPool(rendering->vulkanLogicalDevice, &commandPoolCreateInfo, nullptr, &rendering->frameData[i].vulkanCommandPool) != VK_SUCCESS){
                throw std::runtime_error("Failed to create the command pool!\n");
            }

            // Allocate a command buffer
            VkCommandBufferAllocateInfo allocateInfo = {};
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.pNext = nullptr;
            allocateInfo.commandPool = rendering->frameData[i].vulkanCommandPool;
            allocateInfo.commandBufferCount = 1;
            allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if(vkAllocateCommandBuffers(rendering->vulkanLogicalDevice, &allocateInfo, &rendering->frameData[i].vulkanCommandBuffer) != VK_SUCCESS){
                throw std::runtime_error("Failed to allocate the command buffer!\n");
            }
        }

        return rendering;
    }

    Rendering* InitializeVulkanSyncStructures(Rendering* rendering){
        // Create the render fence, which syncs the CPU with GPU operations
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;

        // Flag indicates we can wait on it before using it on a GPU command (for the first frame)
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        // Create the render and present semaphores, which GPU with GPU operations
        VkSemaphoreCreateInfo semaphorCreateInfo = {};
        semaphorCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphorCreateInfo.pNext = nullptr;
        semaphorCreateInfo.flags = 0;

        for(int i = 0; i < FRAME_OVERLAP; i++){
            if(vkCreateFence(rendering->vulkanLogicalDevice, &fenceCreateInfo, nullptr, &rendering->frameData[i].renderFence) != VK_SUCCESS){
                throw std::runtime_error("Failed to create the render fence!\n");
            }

            if(vkCreateSemaphore(rendering->vulkanLogicalDevice, &semaphorCreateInfo, nullptr, &rendering->frameData[i].swapchainSemaphore) != VK_SUCCESS){
                throw std::runtime_error("Failed to create the present semaphore!\n");
            }

            if(vkCreateSemaphore(rendering->vulkanLogicalDevice, &semaphorCreateInfo, nullptr, &rendering->frameData[i].renderSemaphore) != VK_SUCCESS){
                throw std::runtime_error("Failed to create the render semaphore!\n");
            }
        }

        return rendering;
    }

    FrameData& GetCurrentFrame(Rendering* rendering){
        return rendering->frameData[rendering->frameNumber % FRAME_OVERLAP];
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

    void Draw(Rendering* rendering) {
        // Wait until the GPU has finished rendering the last frame
        if(vkWaitForFences(rendering->vulkanLogicalDevice, 1, &GetCurrentFrame(rendering).renderFence, true, 1000000000) != VK_SUCCESS){
            throw std::runtime_error("Failed to wait on rendering fence!\n");
        }
        if(vkResetFences(rendering->vulkanLogicalDevice, 1, &GetCurrentFrame(rendering).renderFence) != VK_SUCCESS){
            throw std::runtime_error("Failed to reset rendering fence!\n");
        }

        // Request an image from the swapchain that we will use to draw on
        uint32_t swapChainImageIndex;
        if(vkAcquireNextImageKHR(rendering->vulkanLogicalDevice, rendering->vulkanSwapChain, 1000000000, GetCurrentFrame(rendering).swapchainSemaphore, nullptr, &swapChainImageIndex) != VK_SUCCESS){
            throw std::runtime_error("Failed to acquire next swapchain image!\n");
        }

        // Reset the command buffer before adding new commands to it
        if(vkResetCommandBuffer(GetCurrentFrame(rendering).vulkanCommandBuffer, 0) != VK_SUCCESS){
            throw std::runtime_error("Failed to reset the command buffer!\n");
        }

        // Begin recording commands to the buffer
        VkCommandBufferBeginInfo bufferBeginInfo = {};
        bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        bufferBeginInfo.pNext = nullptr;
        bufferBeginInfo.pInheritanceInfo = nullptr;

        // Indicates that the buffer can only be submitted once before it needs to be reset (so basically once per frame)
        bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        // Get the current frame's command buffer
        VkCommandBuffer currentCommandBuffer = GetCurrentFrame(rendering).vulkanCommandBuffer;

        // "Beginning" the command buffer opens it up for recording commands
        if(vkBeginCommandBuffer(currentCommandBuffer, &bufferBeginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to begin the command buffer recording!\n");
        }

        // Make the swapchain image writeable before rendering
        TransitionImage(currentCommandBuffer, rendering->swapChainImages[swapChainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

        // Define the clear color for this frame
        VkClearColorValue clearColorValue;
        float flash = std::abs(std::sin(rendering->frameNumber / 120.0f));
        clearColorValue = {{0.0f, 0.0f, flash, 0.0f}};

        VkImageSubresourceRange clearRange {};
        clearRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clearRange.baseMipLevel = 0;
        clearRange.levelCount = VK_REMAINING_MIP_LEVELS;
        clearRange.baseArrayLayer = 0;
        clearRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        // Add the clear image command to the command buffer
        vkCmdClearColorImage(currentCommandBuffer, rendering->swapChainImages[swapChainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearColorValue, 1, &clearRange);

        // Transition the swapchain image to presentable mode
        TransitionImage(currentCommandBuffer, rendering->swapChainImages[swapChainImageIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        // "End" recording for the command buffer
        if(vkEndCommandBuffer(currentCommandBuffer) != VK_SUCCESS){
            throw std::runtime_error("Failed to end the command buffer recording!\n");
        }

        // Prepare the submission of the command buffer to the queue
        VkCommandBufferSubmitInfo commandBufferSubmitInfo {};
        commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        commandBufferSubmitInfo.pNext = nullptr;
        commandBufferSubmitInfo.commandBuffer = currentCommandBuffer;
        commandBufferSubmitInfo.deviceMask = 0;

        // Define the wait semaphore (wait for the image from the swapchain)
        VkSemaphoreSubmitInfo waitSemaphoreSubmitInfo{};
        waitSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        waitSemaphoreSubmitInfo.pNext = nullptr;
        waitSemaphoreSubmitInfo.semaphore = GetCurrentFrame(rendering).swapchainSemaphore;
        waitSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR;
        waitSemaphoreSubmitInfo.deviceIndex = 0;
        waitSemaphoreSubmitInfo.value = 1;

        // Define the signal semaphore (signal that rendering has finished)
        VkSemaphoreSubmitInfo signalSemaphoreSubmitInfo{};
        signalSemaphoreSubmitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        signalSemaphoreSubmitInfo.pNext = nullptr;
        signalSemaphoreSubmitInfo.semaphore = GetCurrentFrame(rendering).renderSemaphore;
        signalSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
        signalSemaphoreSubmitInfo.deviceIndex = 0;
        signalSemaphoreSubmitInfo.value = 1;

        // Define the submit info which contains both semaphores (wait and signal) along with the command buffer info
        VkSubmitInfo2 submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreInfoCount = 1;
        submitInfo.pWaitSemaphoreInfos = &waitSemaphoreSubmitInfo;
        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.pSignalSemaphoreInfos = &signalSemaphoreSubmitInfo;
        submitInfo.commandBufferInfoCount = 1;
        submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;

        // Submit to the queue and block operations using the rendering fence
        if(vkQueueSubmit2(rendering->vulkanGraphicsQueue, 1, &submitInfo, GetCurrentFrame(rendering).renderFence) != VK_SUCCESS){
            throw std::runtime_error("Failed to submit to the queue!\n");
        }

        // Prepare what was submitted to be presented to the window
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &rendering->vulkanSwapChain;
        presentInfo.pWaitSemaphores = &GetCurrentFrame(rendering).renderSemaphore;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pImageIndices = &swapChainImageIndex;

        if(vkQueuePresentKHR(rendering->vulkanGraphicsQueue, &presentInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to present queue result!\n");
        }

        rendering->frameNumber++;
    }

    void TransitionImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout){
        // This pipeline barrier defines how an image is pulled and placed to/from the swapchain
        VkImageMemoryBarrier2 imageBarrier {};
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        imageBarrier.pNext = nullptr;
        imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
        imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;
        imageBarrier.oldLayout = currentLayout;
        imageBarrier.newLayout = newLayout;

        VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT);

        VkImageSubresourceRange subImage {};
        subImage.aspectMask = aspectMask;
        subImage.baseMipLevel = 0;
        subImage.levelCount = VK_REMAINING_MIP_LEVELS;
        subImage.baseArrayLayer = 0;
        subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

        imageBarrier.subresourceRange = subImage;
        imageBarrier.image = image;

        VkDependencyInfo dependencyInfo {};
        dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependencyInfo.pNext = nullptr;
        dependencyInfo.imageMemoryBarrierCount = 1;
        dependencyInfo.pImageMemoryBarriers = &imageBarrier;

        vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
    }

    void Present() {

    }

    void SetRenderTargets() {
        
    }

    void ClearViews() {
        
    }

    void CleanupRendering(Rendering* rendering) {
        // Wait for the GPU to complete all operations
        vkDeviceWaitIdle(rendering->vulkanLogicalDevice);

        for(int i = 0; i < FRAME_OVERLAP; i++){
            vkDestroyCommandPool(rendering->vulkanLogicalDevice, rendering->frameData[i].vulkanCommandPool, nullptr);

            vkDestroyFence(rendering->vulkanLogicalDevice, rendering->frameData[i].renderFence, nullptr);
            vkDestroySemaphore(rendering->vulkanLogicalDevice, rendering->frameData[i].renderSemaphore, nullptr);
            vkDestroySemaphore(rendering->vulkanLogicalDevice, rendering->frameData[i].swapchainSemaphore, nullptr);
        }

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