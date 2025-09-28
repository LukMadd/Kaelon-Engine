#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "EngineUI.hpp"
#include "Scene/SceneManager.hpp"
#include "Camera/CameraManager.hpp"
#include "Core/RecourseManager.hpp"
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

            VkDescriptorPool createImGuiDescriptorPool();

            void shutDownImGui(VkDescriptorPool &imGuiDescriptorPool);


            private:
                EngineUI engineUI;

                UIInfo uiInfo;

                float scale = 16.0f;

                ImGuiStyle baseStyle;
            
            public:
                EngineObject::Object*& getSelectedObject(){return engineUI.getSelectedObject();}
                EngineScene::Scene* getSelectedScene(){return engineUI.getSelectedScene();}
    };
}


