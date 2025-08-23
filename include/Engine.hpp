#include "SceneManager.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "Camera.hpp"


#include <GLFW/glfw3.h>

namespace Engine{
    struct GameEngine{    
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

            uint32_t currentSceneIndex = 0;
    };
}