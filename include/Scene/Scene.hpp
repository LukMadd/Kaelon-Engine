#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "CameraManager.hpp"
#include "Object.hpp"
#include "RecourseManager.hpp"

namespace EngineScene{
    class Scene{
        public:
            Scene(const std::string &name, int id);
            ~Scene() = default;

            EngineCamera::CameraManager cameraManager;

            std::string name;
            int id;
            std::vector<std::unique_ptr<Object>> objects; 
            SceneNode root;

            bool areObjectsInitialized = false;

            static std::unique_ptr<Scene> createScene(int id, const std::string &name);
            void initBaseScene(EngineResource::ResourceManager &resourceManager);

            int getId() const {return id;}
            std::string getName() const {return name;}

            void update();

            void cleanupObjects();
    };
}

#endif