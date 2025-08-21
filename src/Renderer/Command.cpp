#include "Command.hpp"
#include "RendererGlobals.hpp"
#include <cstdint>
namespace EngineRenderer{
    void Command::createCommandPool(VkSurfaceKHR surface, QueueFamilyIndices queueFamilyIndices){
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        VkResult result = vkCreateCommandPool(device,&poolInfo, nullptr, &commandPool);
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to create command pool error code: " + std::to_string(result) + "!");
        }
    }

    void Command::cleanup(){
        vkDestroyCommandPool(device, commandPool, nullptr);
    }

    void Command::createCommandBuffers(std::vector<VkCommandBuffer> &commandBuffers, int MAX_FRAMES_IN_FLIGHT){
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        VkResult result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
        if(result != VK_SUCCESS){
            throw std::runtime_error("Failed to allocate command buffers error code " + std::to_string(result) + "!");
        }
    }

    void Command::recordCommandBuffers(std::vector<std::unique_ptr<Object>> &objects, VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, SwapChain swapChain, VkPipeline &graphicsPipeline, VkPipelineLayout &pipelineLayout, uint32_t currentFrame, VkBuffer &vertexBuffer, VkBuffer &indexBuffer, const std::vector<uint32_t> &indices, std::vector<VkDescriptorSet> &descriptorSets){
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        VkResult beginResult = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if(beginResult != VK_SUCCESS){
            throw std::runtime_error("Failed to begin command buffersr error code " + std::to_string(beginResult) + "!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChain.swapChainFramebuffers[imageIndex];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};        
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.swapChainExtent.width);
        viewport.height = static_cast<float>(swapChain.swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        for(auto& obj : objects){
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

            obj->draw(commandBuffer, pipelineLayout);
        }
        
        vkCmdEndRenderPass(commandBuffer);

        VkResult endResult = vkEndCommandBuffer(commandBuffer);
        if(endResult != VK_SUCCESS){
            throw std::runtime_error("Failed to record command buffers error code " + std::to_string(endResult) + "!");
        }
    }
}