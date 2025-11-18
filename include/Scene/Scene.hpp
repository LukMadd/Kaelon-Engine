#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Camera/CameraManager.hpp"

#include "Core/ResourceManager.hpp"
#include "ECS/ComponentStorage.hpp"
#include "Scene/SceneNode.hpp"

;

class DebugRenderer;

namespace EngineScene{
    class Scene{
        public:
            Scene(const std::string &name, int index);
            ~Scene() = default;

            EngineCamera::CameraManager cameraManager;

            int index;
            std::string name;
            std::vector<Entity> newEntities;
            std::vector<VkDescriptorSet> descriptorSets;
            ComponentStorage componentStorage;
            SceneNode sceneNode;

            bool areEntitiesInitialized = false;
            bool areDescriptorSetsInitialized = false;
            bool isInitialized = false;

            bool selected = false;

            static std::unique_ptr<Scene> createScene(int id, const std::string &name);
            void initBaseScene(EngineResource::ResourceManager &resourceManager, ECS& ecs);

            void addDefaultEntity(ECS& ecs);

            void removeEntity(Entity e, ECS& ecs);

            void drawBoundingBoxes(ECS& ecs, DebugRenderer *debugRenderer);

            int getIndex() const {return index;}
            std::string getName() const {return name;}

            std::vector<std::shared_ptr<Texture>> getSceneTextures(ECS& ecs);

            void update(ECS& ecs);

            void cleanupEntities();
    };
}

#endif