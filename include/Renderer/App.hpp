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
namespace renderer {
    class App{
        public:
            bool framebuffersrResized = false;

            void run();

        private:
            void initVulkan();
            void mainLoop();
            void createSyncObjects();
            void drawFrame();
            void cleanup();


            Window appWindow;
            GLFWwindow* window = appWindow.getWindow();
            
            VkSurfaceKHR surface;

            Instance appInstance{instance};
            VkInstance instance;

            Queue appQueue;
            QueueFamilyIndices queueFamilyIndices;

            SwapChain appSwapChain{window};
            VkSwapchainKHR swapChain;

            Pipeline appPipeline;

            Vertex vertexCommand;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
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

            modelLoader modelLoader;

            DepthBuffer depthBuffer;

            MipMap mipMap;

            MultiSampler multiSampler;
    };
}