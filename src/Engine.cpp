#include "Engine.hpp"
#include "ECS/Components.hpp"
#include "Input/Input.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererGlobals.hpp"
#include "Misc/ObjectGlobals.hpp"
#include "Debug/Debugger.hpp"
#include "ECS/EntityFunctions.hpp"

#include <chrono>

;
using namespace EngineInput;

namespace Engine{
    GameEngine::GameEngine() : renderer(), sceneManager(){};

    void GameEngine::init(){
        Debugger::get().initDebugSystem("Engine");

        DEBUGGER_LOG(INFO, "SUCESSFULLY CREATED", "Engine");

        renderer.initVulkan();

        spatialPartitioner.init(&ecs);
        sceneManager.init(resourceManager, &spatialPartitioner, &ecs);

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        VkDeviceSize minUboAlignment = deviceProperties.limits.minUniformBufferOffsetAlignment;
        VkDeviceSize stride = sizeof(ObjectUBO);
        if (stride % minUboAlignment != 0) {
            stride += minUboAlignment - (stride % minUboAlignment);
        }

        renderer.setObjectUboStride(stride);

        renderer.initObjectResources(resourceManager);
    
        for(auto &scene : sceneManager.getScenes()){
            renderer.initEntities(*scene, resourceManager, spatialPartitioner, ecs);
        }

        renderer.createSceneDescriptorSets(sceneManager.getCurrentScene(), ecs);

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
        uiInfo.changedBoundingBoxes = &physicsEngine.getChangedBoundingBoxes();
        uiInfo.ecs = &ecs;

        uiManager.initImGui(uiInfo);

        Input::get().setECS(&ecs);
        Input::get().init(window);
        Input::get().setCallBacks();
        actionManager.setupBindings();
        physicsEngine.init(&sceneManager, &spatialPartitioner, &ecs);

        renderer.giveDebugRenderer(&debugRenderer);
    }

    void GameEngine::RendererMainLoop(float deltaTime){
        glfwPollEvents();
        drawCallCountLastFrame = drawCallCount;
        drawCallCount = 0;

        Input::get().inputCamera = sceneManager.getCurrentScene()->cameraManager.getCurrentCamera().get();

        sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->giveExtent(renderer.getSwapChainExtent());

        sceneManager.getCurrentScene()->cameraManager.getCurrentCamera()->updateCamera(deltaTime, actionManager, inputHandler.isSceneImmersed());

        sceneManager.getCurrentScene()->update(ecs); //Updates the current frame's children with it's matrix and so forth

        static float rawFps = 0.0f;
        static float smoothFPS = 0.0f;
        if(rawFps != 0.0f){
            smoothFPS = fpsManager.smoothFPS(rawFps);
        }

        Input::get().selectedEntity = uiManager.getSelectedEntity();
        Input::get().inputScene = sceneManager.getCurrentScene();

        uiManager.renderUI(smoothFPS);

        if(sceneManager.getCurrentScene()->areEntitiesInitialized == false){
            renderer.initEntities(*sceneManager.getCurrentScene(), resourceManager, spatialPartitioner, ecs);
            renderer.createSceneDescriptorSets(sceneManager.getCurrentScene(), ecs);
        }

        if(sceneManager.getCurrentScene()->newEntities.empty() == false){
            while(sceneManager.getCurrentScene()->newEntities.empty() == false){
                auto entity = sceneManager.getCurrentScene()->newEntities.back();
                initResources(entity, ecs, resourceManager, &spatialPartitioner);
                sceneManager.getCurrentScene()->newEntities.pop_back();
            }    
        }

        if(sceneManager.getCurrentScene()->areDescriptorSetsInitialized == false){
            renderer.createSceneDescriptorSets(sceneManager.getCurrentScene(), ecs);
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

        uint32_t i = 0;
        for(auto entity : ecs.view<RenderableComponent, MaterialComponent>()){
            auto* entity_material = ecs.getComponent<MaterialComponent>(entity);
            auto* entity_renderable = ecs.getComponent<RenderableComponent>(entity);

            ObjectUBO objectUbo{};
            objectUbo.hasTexture = entity_material->material->getTextures().empty() ? 0 : 1;
            objectUbo.baseColor  = entity_material->material->getBaseColor();
            int index = 0;
            if(!entity_material->material->getTextures().empty()){
                auto sceneTextures = sceneManager.getCurrentScene()->getSceneTextures(ecs);
                auto it = std::find(sceneTextures.begin(), sceneTextures.end(), entity_material->material->getTextures()[0]);
                if(it != sceneTextures.end()){
                    index = static_cast<int>(std::distance(sceneTextures.begin(), it));
                } else{
                    index = 0; //Fallback if texture was not found
                }
            }
            objectUbo.textureIndex = std::min(index, (int)MAX_TEXTURES - 1);
            
            memcpy((char*)renderer.getObjectUniformBuffersMapped()[renderer.getCurrentFrame()] + i * renderer.getObjectUboStride(), &objectUbo, sizeof(ObjectUBO));

            entity_renderable->uniformIndex = static_cast<uint32_t>(i);

            i++;
        }

        if(uiManager.shouldDrawBoundingBoxes()){
            sceneManager.getCurrentScene()->drawBoundingBoxes(ecs, &debugRenderer);
        }

        FrameFlags frameFlags{};
        frameFlags.shouldDrawBoundingBoxes = uiManager.shouldDrawBoundingBoxes();

        renderer.drawFrame(sceneManager.getCurrentScene(), ecs, frameFlags);
        rawFps = renderer.getGpuFPS();
    }

    void GameEngine::mainLoop(){
        auto lastTime = std::chrono::high_resolution_clock::now();

        while(!glfwWindowShouldClose(window)){            
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
            lastTime = currentTime;

            inputHandler.update(window, actionManager, sceneManager);

            float accumulator;

            float physicsStep = physicsEngine.getTickRate();

            accumulator+=deltaTime;
            while(accumulator >= physicsStep){
                physicsEngine.tick(physicsStep);
                accumulator-=physicsEngine.getTickRate();
            }

            RendererMainLoop(deltaTime); 
            
        }
        vkDeviceWaitIdle(EngineRenderer::device);
    }

    void GameEngine::cleanup(){
        vkDeviceWaitIdle(device);
        sceneManager.saveScenes(); //Serializes all the scenes loaded
        for(auto &scene :  sceneManager.getScenes()){
            scene->cleanupEntities();
        }
        resourceManager.cleanup(device);
        defaultResources.cleanupDefault(); //Destroys the default recourses
        uiManager.shutDownImGui(imguiPool);
        renderer.cleanup(sceneManager.getCurrentScene());
    }
}