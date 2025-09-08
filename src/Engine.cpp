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

        sceneManager.init(resourceManager);

        for(auto &scene : sceneManager.getScenes()){
            scene->update();
        }

        size_t totalObjects = 0;
        for (auto &scene : sceneManager.getScenes()) {
            totalObjects += scene->objects.size();
        }
        totalObjects = std::max(totalObjects, size_t(1));

        renderer.initObjectResources(totalObjects, sceneManager.getCurrentScene()->objects, resourceManager);
    
        for(auto &scene : sceneManager.getScenes()){
            renderer.initObjects(*scene, resourceManager);
        }

        EngineRenderer::DirectionalLight sun;
        sun.direction = glm::normalize(glm::vec3(0.3f, -1.0f, 0.5f));
        sun.color = glm::vec3(1.0f, 0.5f, 0.2f);
        sun.intensity = 0.4f;
        lights.addDirectionalLight(sun);

        imguiPool = uiManager.createImGuiDescriptorPool();
        uiManager.initImGui(window, renderer.getInstance(), VK_NULL_HANDLE, imguiPool, renderer.getRenderPass());

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

            sceneManager.getCurrentScene()->update();

            float fps = fpsManager.updateFPS(deltaTime);

            EngineRenderer::UniformBufferObject ubo{};
            ubo.view = camera.getViewMatrix();

            if(!lights.getDirectionalLights().empty()){
                const auto& directionalLight = lights.getDirectionalLights()[0];
                ubo.lightDir = glm::vec4(glm::normalize(directionalLight.direction), 0.0f);
                ubo.lightColorIntensity = glm::vec4(directionalLight.color, directionalLight.intensity);
            }

            uiManager.beginFrame(window, fps);

            ubo.cameraPos = glm::vec4(camera.position, 0.0f);

            renderer.updateUniformBuffers(ubo);

            renderer.drawFrame(sceneManager.getCurrentScene()->objects, fps);
        }
        vkDeviceWaitIdle(EngineRenderer::device);
    }

    void GameEngine::cleanup(){
        vkDeviceWaitIdle(device);
        sceneManager.saveScenes();
        for(auto &scene :  sceneManager.getScenes()){
            scene->cleanupObjects();
        }
        resourceManager.cleanup(device);
        EngineObject::defaultResources.cleanupDefault();
        uiManager.shutDownImGui(imguiPool);
        renderer.cleanup();
    }
}