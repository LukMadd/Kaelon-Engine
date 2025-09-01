#include "RecourseManager.hpp"
#include "SceneManager.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"


#include <GLFW/glfw3.h>

namespace Engine{
    struct GameEngine{    
        EngineResource::ResourceManager resourceManager;
        SceneManager sceneManager;
        ActionManager actionManager;
        EngineInput::InputHandler inputHandler;
        EngineCamera::Camera camera;

        GameEngine();
    
        void init();

        void mainLoop();

        void cleanup();

        private:
            GLFWwindow *window = nullptr;
            EngineRenderer::Renderer renderer;
            EngineRenderer::Lighting lights;

            uint32_t currentSceneIndex = 0;
    };
}