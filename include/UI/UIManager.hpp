#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "EngineUI.hpp"
#include "SceneManager.hpp"
#include "CameraManager.hpp"
#include "RecourseManager.hpp"
#include "imgui.h"

namespace EngineUI{
    struct UIInfo{
        GLFWwindow* window;
        VkInstance instance;
        VkPipelineCache pipelineCache;
        VkDescriptorPool imGuiDescriptorPool;
        VkRenderPass renderPass;
        EngineScene::SceneManager *sceneManager;
        EngineCamera::CameraManager *cameraManager;
        EngineResource::ResourceManager *recourseManager;
    };

    class UIManager{
        public:
            void initImGui(UIInfo &uiInfoRef);

            void beginFrame(float fps);
            void renderUI(float fps);

            void rebuildImGuiFontAtlas();
            void uploadFontAtlasToGPU();

            VkDescriptorPool createImGuiDescriptorPool();

            void shutDownImGui(VkDescriptorPool &imGuiDescriptorPool);

            private:
                EngineUI engineUI;

                UIInfo uiInfo;

                float scale = 16.0f;

                ImGuiStyle baseStyle;
    };
}


