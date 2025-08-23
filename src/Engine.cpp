#include "Engine.hpp"
#include "Input.hpp"
#include "RendererGlobals.hpp"
#include "ObjectRegistry.hpp"

#include <chrono>

using namespace EngineObject;

namespace Engine{
    GameEngine::GameEngine() : renderer(), camera(), sceneManager(){
        window = renderer.window;
    };

    void GameEngine::init(){
        sceneManager.init();

        size_t totalObjects = 0;
        for (auto &scene : sceneManager.getScenes()) {
            totalObjects += scene.objects.size();
        }
        totalObjects = std::max(totalObjects, size_t(1));
        
        renderer.init(totalObjects, sceneManager.getCurrentScene().objects);
    
        for(auto &scene : sceneManager.getScenes()){
            renderer.initObjects(scene);
        }

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

            inputHandler.update(window, actionManager, sceneManager);

            camera.updateCameraPosition(deltaTime, actionManager);

            EngineRenderer::UniformBufferObject ubo{};
            ubo.view = camera.getViewMatrix();

            renderer.updateUniformBuffers(ubo);

            renderer.drawFrame(sceneManager.getCurrentScene().objects);
        }
        vkDeviceWaitIdle(EngineRenderer::device);
    }

    void GameEngine::cleanup(){
        vkDeviceWaitIdle(device);
        sceneManager.saveScenes();
        for(auto &scene :  sceneManager.getScenes()){
            scene.cleanupObjects();
        }
        renderer.cleanup();
    }
}