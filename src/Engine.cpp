#include "Engine.hpp"
#include "Input.hpp"
#include "RendererGlobals.hpp"
#include "ObjectRegistry.hpp"
#include "ObjectGlobals.hpp"

#include <chrono>

using namespace EngineObject;

namespace Engine{
    GameEngine::GameEngine() : renderer(), sceneManager(){};

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

        window = renderer.window;

        EngineRenderer::DirectionalLight sun;
        sun.direction = glm::normalize(glm::vec3(0.3f, -1.0f, 0.5f));
        sun.color = glm::vec3(1.0f, 0.5f, 0.2f);
        sun.intensity = 0.4f;
        lights.addDirectionalLight(sun);

        imguiPool = uiManager.createImGuiDescriptorPool();
        
        EngineUI::UIInfo uiInfo{};
        uiInfo.imGuiDescriptorPool = imguiPool;
        uiInfo.window = window;
        uiInfo.instance = renderer.getInstance();
        uiInfo.pipelineCache = nullptr;
        uiInfo.renderPass = renderer.getRenderPass();
        uiInfo.sceneManager = &sceneManager;
        uiInfo.cameraManager = &sceneManager.getScenes()[0]->cameraManager;
        uiInfo.recourseManager = &resourceManager;

        uiManager.initImGui(uiInfo);

        Input::get().init(window);
        Input::get().setCallBacks();
        actionManager.setupBindings();
    }

    void GameEngine::mainLoop(){
        auto lastTime = std::chrono::high_resolution_clock::now();

        while(!glfwWindowShouldClose(window)){
            glfwPollEvents();
            drawCallCountLastFrame = drawCallCount;
            drawCallCount = 0;

            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
            lastTime = currentTime;

            inputHandler.update(window, actionManager, sceneManager);

            sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->updateCameraPosition(deltaTime, actionManager, inputHandler.isSceneImmersed());

            sceneManager.getCurrentScene()->update(); //Updates the current frame's children with it's matrix and so forth

            float fps = fpsManager.updateFPS(deltaTime);

            uiManager.renderUI(fps);

                EngineRenderer::UniformBufferObject ubo{};
                ubo.view = sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->getViewMatrix();
                if(!lights.getDirectionalLights().empty()){
                    const auto& directionalLight = lights.getDirectionalLights()[0];
                    ubo.lightDir = glm::vec4(glm::normalize(directionalLight.direction), 0.0f);
                    ubo.lightColorIntensity = glm::vec4(directionalLight.color, directionalLight.intensity);
                }

                ubo.cameraPos = glm::vec4(sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->position, 0.0f); //Updates the camera position
                renderer.updateUniformBuffers(ubo, sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->getFov()); //Sends the uniform buffer object down to the uniform buffer manager for it to be processed
            
            for(auto &object : sceneManager.getCurrentScene()->objects){
                ObjectUBO objectUbo;

<<<<<<< HEAD
                objectUbo.hasTexture = object->material->isTexturesEmpty() ? 0 : 1;
=======
                objectUbo.hasTexture = object->material->getTextures().empty() ? 0 : 1;
>>>>>>> aaa4a49 (Added support for objects to have a color instead of a texture)

                objectUbo.baseColor = object->material->getBaseColor();

                renderer.updateObjectUniformBuffers(objectUbo);
            }   

            renderer.drawFrame(sceneManager.getCurrentScene()->objects, fps);
        }
        vkDeviceWaitIdle(EngineRenderer::device);
    }

    void GameEngine::cleanup(){
        vkDeviceWaitIdle(device);
        sceneManager.saveScenes(); //Serializes all the scenes loaded
        for(auto &scene :  sceneManager.getScenes()){
            scene->cleanupObjects();
        }
        resourceManager.cleanup(device);
        EngineObject::defaultResources.cleanupDefault(); //Destroys the default recourses
        uiManager.shutDownImGui(imguiPool);
        renderer.cleanup(sceneManager.getCurrentScene());
    }
}