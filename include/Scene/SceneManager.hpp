
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

            void init(EngineRecourse::RecourseManager &recourseManager);

            void addScene();
            void changeScenes(int indexChange);

            std::vector<Scene> &getScenes();
            Scene &getCurrentScene();

            json serializeObject(const Object &object);
            Object* deserializeObject(const nlohmann::json& jsonData);
            void serializeScene(Scene &scene, uint32_t sceneIndex);
            void deserializeScene(const std::string& filename);

            void saveScenes();
            void cleanup();

            uint32_t currentSceneIndex = 0;

        private:
            EngineRecourse::RecourseManager *recourseManager;

            std::vector<Scene> scenes;
    };
}

#endif