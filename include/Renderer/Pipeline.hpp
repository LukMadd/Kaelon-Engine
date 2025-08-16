#include <vulkan/vulkan.hpp>

namespace renderer {
    class Pipeline{
        public:
            VkPipelineLayout pipelineLayout;

            VkPipeline graphicsPipeline;

            VkRenderPass renderPass;

            void createGraphicsPipeline(VkExtent2D swapChainExtent, VkDescriptorSetLayout descriptorSetLayout);

            void createRenderPass(VkFormat swapChainImageFormat);

            void cleanupPipeline();

        private:
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
        };
}