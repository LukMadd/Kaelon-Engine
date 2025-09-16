#ifndef _PIPELINE_HPP
#define _PIPELINE_HPP

#include <vulkan/vulkan.hpp>

namespace EngineRenderer {
    
   
    struct PipelineInfo{ 
        bool wireFrameMode = false;
    }; //Will be eventually expanded to include more than just this

    class PipelineManager{
        public:
            VkPipelineLayout pipelineLayout;

            VkPipeline graphicsPipelineFill;
            VkPipeline graphicsPipelineWireFrame;
            
            VkRenderPass renderPass;

            void createGraphicsPipeline(VkPipeline &pipeline, PipelineInfo pipelineInfo, std::string name, VkExtent2D swapChainExtent, VkDescriptorSetLayout descriptorSetLayout);

            void pushPipeline(VkPipeline &pipeline){pipelines.push_back(&pipeline);}

            void createPipelines(VkExtent2D swapChainExtent, VkDescriptorSetLayout globalDescriptorSetLayout);

            void createRenderPass(VkFormat swapChainImageFormat);

            void cleanupPipelines();

        private:
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

            std::vector<VkPipeline*> pipelines;
        };
}

#endif