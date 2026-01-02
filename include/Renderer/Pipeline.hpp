#ifndef _PIPELINE_HPP
#define _PIPELINE_HPP

#include <map>
#include <vulkan/vulkan.hpp>
#include "Debug/Debugger.hpp"

constexpr const char* DEBUG_SYSTEM = "Renderer";

namespace EngineRenderer {
    struct PipelineInfo{ 
        bool wireFrameMode = false;
        std::string fragShader = "";
        std::string vertShader = "";
        bool depthEnabled = true;
        VkPipelineLayout pipelineLayout;
        bool minimalAttributeDescription = false;
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkCullModeFlags cullMode = VK_CULL_MODE_NONE;
    }; //Will be eventually expanded to include more than just this

    class PipelineManager{
        public:
            VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
            VkPipelineLayout debugLayout = VK_NULL_HANDLE;

            std::map<std::string, VkPipeline*> pipelines;

            VkPipeline graphicsPipelineFill;
            VkPipeline graphicsPipelineWireFrame;
            VkPipeline graphicsPipelineDebug;
            
            VkRenderPass renderPass;

            void createGraphicsPipeline(VkPipeline &pipeline, PipelineInfo pipelineInfo, const char* name, VkExtent2D swapChainExtent);

            void pushPipeline(VkPipeline &pipeline, const char* name){
                if(pipelines[name]){
                    DEBUGGER_LOG(WARNING, "Pipeline already exists in pipelines", DEBUG_SYSTEM);
                    return;
                }
                pipelines[name] = (&pipeline);
            }

            void createPipelines(VkExtent2D swapChainExtent, VkDescriptorSetLayout globalDescriptorSetLayout);

            void createRenderPass(VkFormat swapChainImageFormat);

            void cleanupPipelines();

            std::vector<VkPipeline*> getPipelines() {
                std::vector<VkPipeline*> pipelinesVector;
                pipelinesVector.reserve(pipelines.size());
                for(auto &pipeline : pipelines){
                    pipelinesVector.push_back(pipeline.second);
                }

                return pipelinesVector;
            }

        private:
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

        };
}

#endif