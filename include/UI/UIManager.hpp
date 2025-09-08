#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

namespace EngineUI{
    class UIManager{
        public:
            void initImGui(
            GLFWwindow* window, 
            VkInstance instance, 
            VkPipelineCache pipelineCache, 
            VkDescriptorPool imGuiDescriptorPool, 
            VkRenderPass renderPass);

            void beginFrame(GLFWwindow* window, float fps);

            VkDescriptorPool createImGuiDescriptorPool();

            void shutDownImGui(VkDescriptorPool &imGuiDescriptorPool);
    };
}


