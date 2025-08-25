#include "Engine.hpp"
#include "Input.hpp"
#include "RendererGlobals.hpp"
#include "ObjectRegistry.hpp"
#include "ObjectGlobals.hpp"

#include <chrono>

using namespace EngineObject;

namespace Engine{
    GameEngine::GameEngine() : renderer(), camera(), sceneManager(){
        window = renderer.window;
    };

    void GameEngine::init(){
        renderer.initVulkan();

        sceneManager.init(recourseManager);

        for(auto &scene : sceneManager.getScenes()){
            scene.update();
        }

        size_t totalObjects = 0;
        for (auto &scene : sceneManager.getScenes()) {
            totalObjects += scene.objects.size();
        }
        totalObjects = std::max(totalObjects, size_t(1));
        
        renderer.initObjectRecourses(totalObjects, sceneManager.getCurrentScene().objects);
    
        for(auto &scene : sceneManager.getScenes()){
            renderer.initObjects(scene, recourseManager);
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

            sceneManager.getCurrentScene().update();

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
        EngineObject::defaultResources.cleanupDefault();
        renderer.cleanup();
    }
}