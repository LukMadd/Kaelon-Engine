#include "Engine.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "RendererGlobals.hpp"
#include "ObjectRegistry.hpp"
#include "ObjectGlobals.hpp"

#include <chrono>

using namespace EngineObject;
using namespace EngineInput;

namespace Engine{
    GameEngine::GameEngine() : renderer(), sceneManager(){};

    void GameEngine::init(){
        renderer.initVulkan();

        sceneManager.init(resourceManager);

        for(auto &scene : sceneManager.getScenes()){
            scene->update();
        }

        for(auto &scene : sceneManager.getScenes()){
            totalObjects += scene->objects.size();
        }
        totalObjects = std::max(totalObjects, size_t(1));

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        VkDeviceSize minUboAlignment = deviceProperties.limits.minUniformBufferOffsetAlignment;
        VkDeviceSize stride = sizeof(ObjectUBO);
        if (stride % minUboAlignment != 0) {
            stride += minUboAlignment - (stride % minUboAlignment);
        }

        renderer.setObjectUboStride(stride);

        renderer.initObjectResources(totalObjects,resourceManager);
    
        for(auto &scene : sceneManager.getScenes()){
            renderer.initObjects(*scene, resourceManager);
        }

        renderer.createSceneDescriptorSets(sceneManager.getCurrentScene());

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

    void GameEngine::RendererMainLoop(std::chrono::time_point<std::chrono::high_resolution_clock>& lastTime){
        glfwPollEvents();
        drawCallCountLastFrame = drawCallCount;
        drawCallCount = 0;

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        inputHandler.update(window, actionManager, sceneManager);

        Input::get().inputCamera = sceneManager.getCurrentScene()->cameraManager.getCurrentCamera().get();

        sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->giveExtent(renderer.getSwapChainExtent());

        sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->updateCamera(deltaTime, actionManager, inputHandler.isSceneImmersed());

        sceneManager.getCurrentScene()->update(); //Updates the current frame's children with it's matrix and so forth

        static float rawFps = 0.0f;
        static float smoothFPS = 0.0f;
        if(rawFps != 0.0f){
            smoothFPS = fpsManager.smoothFPS(rawFps);
        }

        Input::get().selectedObject = &uiManager.getSelectedObject();
        Input::get().inputScene = sceneManager.getCurrentScene();

        uiManager.renderUI(smoothFPS);

        if(sceneManager.getCurrentScene()->areObjectsInitialized == false){
            totalObjects+=sceneManager.getCurrentScene()->objects.size();
            renderer.initObjects(*sceneManager.getCurrentScene(), resourceManager);
            renderer.createSceneDescriptorSets(sceneManager.getCurrentScene());
        }

        if(sceneManager.getCurrentScene()->newObjects.empty() == false){
            while(sceneManager.getCurrentScene()->newObjects.empty() == false){
                auto object = sceneManager.getCurrentScene()->newObjects.back();
                object->initVulkanResources(resourceManager);
                sceneManager.getCurrentScene()->newObjects.pop_back();
            }    
        }

        if(sceneManager.getCurrentScene()->areDescriptorSetsInitialized == false){
            renderer.createSceneDescriptorSets(sceneManager.getCurrentScene());
        }

        EngineRenderer::UniformBufferObject ubo{};
        ubo.view = sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->getViewMatrix();
        if(!lights.getDirectionalLights().empty()){
            const auto& directionalLight = lights.getDirectionalLights()[0];
            ubo.lightDir = glm::vec4(glm::normalize(directionalLight.direction), 0.0f);
            ubo.lightColorIntensity = glm::vec4(directionalLight.color, directionalLight.intensity);
        }

        ubo.proj = sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->getProjection();
        ubo.cameraPos = glm::vec4(sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->position, 0.0f); //Updates the camera position
        renderer.updateUniformBuffers(ubo, sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->getFov()); //Sends the uniform buffer object down to the uniform buffer manager for it to be processed
        
        auto* mappedBuffer = reinterpret_cast<ObjectUBO*>(renderer.getObjectUniformBuffersMapped()[renderer.getCurrentFrame()]);

        for(size_t i = 0; i < sceneManager.getCurrentScene()->objects.size(); i++){
            auto &object = sceneManager.getCurrentScene()->objects[i];

            ObjectUBO objectUbo{};
            objectUbo.hasTexture = object->material->getTextures().empty() ? 0 : 1;
            objectUbo.baseColor  = object->material->getBaseColor();
            int index = 0;
            if(!object->material->getTextures().empty()){
                auto sceneTextures = sceneManager.getCurrentScene()->getSceneTextures();
                auto it = std::find(sceneTextures.begin(), sceneTextures.end(), object->material->getTextures()[0]);
                if(it != sceneTextures.end()){
                    index = static_cast<int>(std::distance(sceneTextures.begin(), it));
                } else{
                    index = 0; //Fallback if texture was not found
                }
            }
            objectUbo.textureIndex = std::min(index, (int)MAX_TEXTURES - 1);
            
            memcpy((char*)renderer.getObjectUniformBuffersMapped()[renderer.getCurrentFrame()] + i * renderer.getObjectUboStride(), &objectUbo, sizeof(ObjectUBO));

            object->uniformIndex =  static_cast<uint32_t>(i);
        }

        renderer.drawFrame(sceneManager.getCurrentScene());
        rawFps = renderer.getGpuFPS();
    }

    void GameEngine::mainLoop(){
        auto lastTime = std::chrono::high_resolution_clock::now();

        while(!glfwWindowShouldClose(window)){
            RendererMainLoop(lastTime);
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