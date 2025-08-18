#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include "Queue.hpp"
#include "DepthBuffer.hpp"

namespace renderer {
    class SwapChainSupportDetails{
            public:
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
            
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
};

    class SwapChain{
        public:
            VkExtent2D swapChainExtent;

            VkFormat swapChainImageFormat;

            std::vector<VkImage> swapChainImages;

            std::vector<VkFramebuffer> swapChainFramebuffers;

            SwapChain(GLFWwindow* window);

            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

            VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& availablePresent);

            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabillities);

            void createSwapChain(VkSurfaceKHR surface, QueueFamilyIndices indices);

            void createImageViews();    

            void createFramebuffers(VkRenderPass renderPass, VkImageView depthImageView);

            VkSwapchainKHR getSwapChain();

            void recreateSwapChain(VkSurfaceKHR surface,QueueFamilyIndices indices, VkRenderPass renderPass, VkCommandPool commandPool, DepthBuffer &depthBuffer, GLFWwindow* window);
            void cleanupSwapChain(DepthBuffer &depthBuffer);

        private:
            GLFWwindow* m_window;
            VkSwapchainKHR m_swapChain;

            std::vector<VkImageView> m_swapChainImageViews;
    };
}