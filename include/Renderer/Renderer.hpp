#ifndef _RENDERER_HPP
#define _RENDERER_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Instance.hpp"
#include "Pipeline.hpp"
#include "Command.hpp"
#include "Vertex.hpp"
#include "UniformBuffer.hpp"
#include "ModelLoader.hpp"
#include "TextureLoader.hpp"
#include "DepthBuffer.hpp"
#include "MipMap.hpp"
#include "MultiSampling.hpp"
#include "Utility.hpp"
#include "Object.hpp"

using namespace EngineScene;
namespace EngineRenderer {
    class Renderer{
        private:
            void initVulkan();
            void mainLoop(std::vector<std::unique_ptr<Object>> objects);
            void createSyncObjects();

            Window appWindow;
            
            VkSurfaceKHR surface;

            VkInstance instance;
            Instance appInstance{instance};

            Queue appQueue;
            QueueFamilyIndices queueFamilyIndices;

            SwapChain appSwapChain;
            VkSwapchainKHR swapChain;

            Pipeline appPipeline;

            Vertex vertexCommand;
            VertexBuffer vertexBufferCommand;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            IndexBuffer indexBufferCommand;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            UniformBuffer uniformBufferCommand;
            VkDescriptorSetLayout descriptorSetLayout;
            VkDescriptorPool descriptorPool;
            std::vector<VkDescriptorSet> descriptorSets;
            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;

            Command appCommand;
            std::vector<VkCommandBuffer> commandbuffers;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            uint32_t currentFrame = 0;

            TextureLoader textureLoader;
            VkImage textureImage;
            VkDeviceMemory textureImageMemory;
            VkImageView textureImageView;
            VkSampler textureSampler;

            ModelLoader modelLoader;

            DepthBuffer depthBuffer;

            MipMap mipMap;

            MultiSampler multiSampler;

            public: 
                Renderer();

                GLFWwindow* window;
                bool framebuffersrResized = false;

                void init();
                void cleanup();
                void drawFrame(std::vector<std::unique_ptr<EngineScene::Object>>& objects);

                void updateUniformBuffers(UniformBufferObject &ubo){
                    uniformBufferCommand.updateUniformBuffers(ubo, currentFrame, appSwapChain.swapChainExtent, uniformBuffersMapped);
                }

    };
}

#endif