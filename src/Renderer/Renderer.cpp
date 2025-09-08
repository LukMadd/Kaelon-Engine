#include "Renderer.hpp"
#include "Instance.hpp"
#include "RecourseManager.hpp"
#include "ObjectGlobals.hpp"
#include "RendererGlobals.hpp"
#include <cstdint>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace EngineRenderer{
    Renderer::Renderer() : appWindow(){
        window = appWindow.getWindow();
        appSwapChain.initSwapChain(window);
    }

    void Renderer::initVulkan(){
        glfwSetWindowUserPointer(appWindow.getWindow(), this);
        appInstance.createInstance(instance);
        appWindow.createSurface(instance, window,surface);
        appInstance.pickPhysicalDevice(instance, surface);
        queueFamilyIndices = appQueue.FindQueueFamily(physicalDevice, surface);
        EngineRenderer::device = appInstance.createLogicalDevice(surface, queueFamilyIndices);
        vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
        appSwapChain.createSwapChain(surface, queueFamilyIndices);
        swapChain = appSwapChain.getSwapChain();
        appSwapChain.createImageViews();
        renderFinishedSemaphores.resize(appSwapChain.swapChainImages.size());        
        appPipeline.createRenderPass(appSwapChain.swapChainImageFormat);
        appCommand.createCommandPool(surface, queueFamilyIndices);
    }
    
    void Renderer::initObjectResources(uint32_t objectCount, std::vector<std::unique_ptr<EngineScene::Object>>& objects, EngineResource::ResourceManager &resourceManager){
        defaultResources.init(resourceManager);
        dummyresources.createDummyresources();
        auto layout = uniformBufferCommand.createDescriptorSetLayout(descriptorSetLayout);
        descriptorLayouts.push_back(layout);
        appPipeline.createGraphicsPipeline(appSwapChain.swapChainExtent, descriptorSetLayout);
        multiSampler.createColorResources(appSwapChain.swapChainImageFormat, appSwapChain.swapChainExtent);
        depthBuffer.createDepthResources(appSwapChain.swapChainExtent, depthBuffer.depthImage, depthBuffer.depthImageMemory, depthBuffer.depthImageView);
        appSwapChain.createFramebuffers(appPipeline.renderPass, depthBuffer.depthImageView, multiSampler.colorImageView);
        uniformBufferCommand.createUniformBuffers(MAX_FRAMES_IN_FLIGHT, uniformBuffers, uniformBuffersMemory, uniformBuffersMapped);
        uniformBufferCommand.createDescriptorPool(objectCount, MAX_FRAMES_IN_FLIGHT, descriptorPool);
        appCommand.createCommandBuffers(commandbuffers, MAX_FRAMES_IN_FLIGHT);
        createSyncObjects();
    }

    void Renderer::initObjects(Scene &scene, EngineResource::ResourceManager &resourceManager){
        for(auto &obj : scene.objects){
            obj->initVulkanResources(resourceManager);

            auto layout = uniformBufferCommand.createDescriptorSetLayout(descriptorSetLayout);
            obj->descriptorSetLayout = layout;
            descriptorLayouts.push_back(layout);
            std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
            obj->descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
            
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
            allocInfo.pSetLayouts = layouts.data();

            if (vkAllocateDescriptorSets(device, &allocInfo, obj->descriptorSets.data()) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate descriptor sets for object");
            }

            uniformBufferCommand.createDescriptorSets(MAX_FRAMES_IN_FLIGHT, uniformBuffers, layout, descriptorPool, obj->descriptorSets, obj->material->getTextures());
        }
        scene.isInitialized = true;
    }

    void Renderer::createSyncObjects(){
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS){
                throw std::runtime_error("Failed to create semaphores!");
            }
        }   

        for(size_t i = 0; i < renderFinishedSemaphores.size(); i++){
            if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS){
                throw std::runtime_error("Failed to create renderFinished semaphore!");
            }
        }
    }
    
    void Renderer::drawFrame(std::vector<std::unique_ptr<EngineScene::Object>>& objects, float fps){
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            framebuffersrResized = false;
            appSwapChain.recreateSwapChain(surface, queueFamilyIndices, appPipeline.renderPass, depthBuffer, multiSampler, appWindow.getWindow());
            swapChain = appSwapChain.getSwapChain();
            return;
        } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandbuffers[currentFrame], 0);

        appCommand.recordCommandBuffers(objects, commandbuffers[currentFrame], imageIndex, appPipeline.renderPass, appSwapChain, appPipeline.graphicsPipeline, appPipeline.pipelineLayout, currentFrame, vertexBuffer, indexBuffer, fps);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandbuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[imageIndex]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VkResult submitResult = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
        if(submitResult != VK_SUCCESS){
            throw std::runtime_error("Failed to submit queue error code: " + std::to_string(submitResult) + "!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(presentQueue, &presentInfo);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffersrResized){
            framebuffersrResized = false;
            appSwapChain.recreateSwapChain(surface, queueFamilyIndices, appPipeline.renderPass, depthBuffer,multiSampler, appWindow.getWindow());
            swapChain = appSwapChain.getSwapChain();
            return;
        } else if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::cleanup(){
        appSwapChain.cleanupSwapChain(depthBuffer, multiSampler);
        depthBuffer.cleanup();

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

        for (size_t i = 0; i < renderFinishedSemaphores.size(); i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        }

        appCommand.cleanup();

        appPipeline.cleanupPipeline();
        
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        for(auto &layout : descriptorLayouts){
            vkDestroyDescriptorSetLayout(device, layout, nullptr);
        }

        vkDestroyImageView(device, dummyresources.texture->textureImageView, nullptr);
        vkDestroyImage(device, dummyresources.texture->textureImage, nullptr);
        vkFreeMemory(device, dummyresources.texture->textureImageMemory, nullptr);
        vkDestroySampler(device, dummyresources.texture->textureSampler, nullptr);
       
        vkDestroyDevice(device, nullptr);

        vkDestroySurfaceKHR(instance, surface, nullptr);

        auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(destroyFunc){
            destroyFunc(instance, debugMessenger, nullptr);
        }

        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
}