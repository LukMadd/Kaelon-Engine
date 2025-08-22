#include "Engine.hpp"
#include "Input.hpp"
#include "RendererGlobals.hpp"

#include <chrono>

namespace Engine{
    GameEngine::GameEngine() : renderer(), camera(){
        window = renderer.window;
    };

    void GameEngine::init(){
        scene.initScene();
        renderer.init(scene.objects);
        renderer.initObjects(scene.objects);
        Input::get().init(window);
        Input::get().setCallBacks();
        actionManager.setupBindings();
    }

    void GameEngine::mainLoop(){
        auto lastTime = std::chrono::high_resolution_clock::now();

        while(!glfwWindowShouldClose(window)){
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
            lastTime = currentTime;

            inputHandler.update(window, actionManager);

            camera.updateCameraPosition(deltaTime, actionManager);

            EngineRenderer::UniformBufferObject ubo{};
            ubo.view = camera.getViewMatrix();

            renderer.updateUniformBuffers(ubo);

            renderer.drawFrame(scene.objects);
        }
        vkDeviceWaitIdle(EngineRenderer::device);
    }

    void GameEngine::cleanup(){
        vkDeviceWaitIdle(device);
        scene.cleanupObjects();
        renderer.cleanup();
    }
}