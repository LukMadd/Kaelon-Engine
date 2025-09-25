#ifndef _COMMAND_HPP
#define _COMMAND_HPP


#include <vulkan/vulkan.hpp>
#include "Scene.hpp"
#include "SwapChain.hpp"
#include "Queue.hpp"

using namespace EngineScene;

namespace EngineRenderer{
    class Command{
        public:
            void createCommandPool(VkSurfaceKHR surface, QueueFamilyIndices queueFamilyIndices);

            void cleanup();

            void createCommandBuffers(std::vector<VkCommandBuffer> &commandBuffers, int MAX_FRAMES_IN_FLIGHT);

            void recordCommandBuffers(
                EngineScene::Scene *scene, 
                VkCommandBuffer commandBuffer, 
                uint32_t imageIndex, 
                VkRenderPass renderPass, 
                SwapChain swapChain, 
                VkPipeline &graphicsPipeline, 
                VkPipelineLayout &pipelineLayout, 
                uint32_t currentFrame, 
                VkBuffer &vertexBuffer, 
                VkBuffer &indexBuffer, 
                VkQueryPool &queryPool,
                VkDeviceSize objectUboStride);
    };
}

#endif