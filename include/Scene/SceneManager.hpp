
#ifndef _SCENE_MANAGER_HPP
#define _SCENE_MANAGER_HPP

#include <cstdint>
#include "Scene.hpp"
#include "nlohmann/json.hpp"
#include "RecourseManager.hpp"

using namespace nlohmann;

namespace EngineScene{
    class SceneManager{
        public:
            SceneManager();

            void init(EngineResource::ResourceManager &resourceManager);

            void addScene(const std::string &name, int id);
            void changeScenes(int indexChange);

            std::vector<Scene*> getScenes();
            Scene* getCurrentScene();

            json serializeObject(const Object &object);
            json serializeNode(SceneNode* node);
            Object* deserializeObject(const nlohmann::json& jsonData);
            SceneNode* deserializeNode(Scene &scene, const json& jsonNode);
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