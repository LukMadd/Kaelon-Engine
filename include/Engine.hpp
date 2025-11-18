#include "Core/ResourceManager.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer/Renderer.hpp"
#include "Input/InputHandler.hpp"
#include "Input/Action.hpp"
#include "Renderer/Lighting.hpp"
#include "UI/UIManager.hpp"
#include "UI/FPSManager.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Debug/DebugRenderer.hpp"
#include "ECS/ECS.hpp"

#include <GLFW/glfw3.h>


namespace Engine{
    struct GameEngine{    
        EngineResource::ResourceManager resourceManager;
        SceneManager sceneManager;
        EngineInput::ActionManager actionManager;
        EngineInput::InputHandler inputHandler;
        EnginePhysics::PhysicsEngine physicsEngine;
        EngineUI::UIManager uiManager;
        EngineUI::FPSManager fpsManager; //Might just become debug manager in the future

        GameEngine();
    
        void init();

        void RendererMainLoop(float deltaTime);

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

            EnginePartitioning::Spatial_Partitioner spatialPartitioner;
            DebugRenderer debugRenderer;

            uint32_t currentSceneIndex = 0;
            size_t totalObjects;

            ECS ecs;
    };
}