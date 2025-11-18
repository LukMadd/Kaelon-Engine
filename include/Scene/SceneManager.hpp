
#ifndef _SCENE_MANAGER_HPP
#define _SCENE_MANAGER_HPP

#include "Scene.hpp"
#include "Core/ResourceManager.hpp"
#include "Spatial/Spatial_Partitioner.hpp"
#include "ECS/ECS.hpp"
#include <cstdint>

namespace EngineScene{
    class SceneManager{
        public:
            SceneManager();

            void init(EngineResource::ResourceManager &resourceManager, 
                     EnginePartitioning::Spatial_Partitioner *spatialPartitioner,
                     ECS *ecs);

            void addDefaultScene();
            void changeScenes(int index);

            std::vector<Scene*> getScenes();
            Scene* getCurrentScene();
            int getCurrentSceneIndex(){return currentSceneIndex;}

            void serializeScene(Scene &scene, uint32_t sceneIndex);
            void deserializeScene(const std::string& filename);
            
            void deleteScene(Scene *scene);

            Scene* getScene(int id);

            void saveScenes();
            void cleanup();

        private:
            EngineResource::ResourceManager *resourceManager;

            EnginePartitioning::Spatial_Partitioner *spatialPartitioner;

            std::unordered_map<int, std::unique_ptr<Scene>> scenes;

            std::vector<int> sceneOrder;

            int currentSceneIndex = 0;

            int currentID = 0;

            ECS* ecs;
    };
}

#endif