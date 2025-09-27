#include "RecourseManager.hpp"
#include "SceneManager.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "Action.hpp"
#include "Lighting.hpp"
#include "UIManager.hpp"
#include "FPSManager.hpp"

#include <GLFW/glfw3.h>


namespace Engine{
    struct GameEngine{    
        EngineResource::ResourceManager resourceManager;
        SceneManager sceneManager;
        EngineInput::ActionManager actionManager;
        EngineInput::InputHandler inputHandler;
        EngineUI::UIManager uiManager;
        EngineUI::FPSManager fpsManager; //Might just become debug manager in the future

        GameEngine();
    
        void init();

        void RendererMainLoop(std::chrono::time_point<std::chrono::high_resolution_clock>& lastTime);

        void SaveScenes(){
            sceneManager.saveScenes();
        }

        void mainLoop();

        void cleanup();

        private:
            GLFWwindow *window = nullptr;
            EngineRenderer::Renderer renderer;
            EngineRenderer::Lighting lights;
            VkDescriptorPool imguiPool;

            uint32_t currentSceneIndex = 0;
            size_t totalObjects;
    };
}