#include "Scene.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "Camera.hpp"

#include <GLFW/glfw3.h>

namespace Engine{
    struct GameEngine{    
        EngineScene::Scene scene;
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
    };
}