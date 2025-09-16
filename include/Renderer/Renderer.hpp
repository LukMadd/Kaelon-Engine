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
#include "RendererUtilities.hpp"
#include "Object.hpp"
#include "Scene.hpp"

using namespace EngineScene;
namespace EngineRenderer {
    class Renderer{
        private:
            void createSyncObjects();

            Window appWindow;
            
            VkSurfaceKHR surface;

            VkInstance instance;
            Instance appInstance{instance};

            Queue appQueue;

            SwapChain appSwapChain;
            VkSwapchainKHR swapChain;

            PipelineManager appPipeline;

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
            std::vector<VkDescriptorSetLayout> descriptorLayouts;
            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;

            std::vector<VkBuffer> objectUniformBuffers;
            std::vector<VkDeviceMemory> objectUniformBuffersMemory;
            std::vector<void*> objectUniformBuffersMapped;

            Command appCommand;
            std::vector<VkCommandBuffer> commandbuffers;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            uint32_t currentFrame = 0;

            public: 
                Renderer();

                GLFWwindow* window;
                bool framebuffersrResized = false;

                void initVulkan();
                void initObjectResources(uint32_t objectCount, std::vector<std::unique_ptr<EngineScene::Object>>& objects, EngineResource::ResourceManager &resourceManager);
                void initObjects(Scene &scene, EngineResource::ResourceManager &resourceManage);
                void initSceneDescriptors(std::vector<std::unique_ptr<EngineScene::Object>>& objects);
                void cleanup(Scene *scene);
                void drawFrame(std::vector<std::unique_ptr<EngineScene::Object>>& objects, float fps);

                void updateUniformBuffers(UniformBufferObject &ubo, float fov){
                    uniformBufferCommand.updateUniformBuffers(ubo, fov, currentFrame, appSwapChain.swapChainExtent, uniformBuffersMapped);
                }

                void updateObjectUniformBuffers(ObjectUBO &objectUBO){
                    uniformBufferCommand.updateObjectUniformBuffers(objectUBO, objectUniformBuffersMapped, currentFrame);
                }

                VkInstance& getInstance(){return instance;}
                VkDescriptorPool& getDescriptorPool(){return descriptorPool;}
                VkRenderPass& getRenderPass(){return appPipeline.renderPass;}
                VkCommandBuffer& getCurrentCommandBuffer(){return commandbuffers[currentFrame];}

    };
}

#endif