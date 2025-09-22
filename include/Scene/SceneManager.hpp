
#ifndef _SCENE_MANAGER_HPP
#define _SCENE_MANAGER_HPP

#include "Scene.hpp"
#include "RecourseManager.hpp"

namespace EngineScene{
    class SceneManager{
        public:
            SceneManager();

            void init(EngineResource::ResourceManager &resourceManager);

            void addDefaultScene();
            void changeScenes(int indexChange);

            std::vector<Scene*> getScenes();
            Scene* getCurrentScene();

            void serializeScene(Scene &scene, uint32_t sceneIndex);
            void deserializeScene(const std::string& filename);

            Scene* getScene(int id);

            void saveScenes();
            void cleanup();

        private:
            EngineResource::ResourceManager *resourceManager;

            std::unordered_map<int, std::unique_ptr<Scene>> scenes;

            std::vector<int> sceneOrder;

            int currentSceneIndex = 0;

            int currentID = 0;
    };
}

#endif