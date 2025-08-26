#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Object.hpp"
#include "RecourseManager.hpp"
#include "SceneNode.hpp"

namespace EngineScene{
    class Scene{
        public:
            Scene(const std::string &name, int id);
            ~Scene() = default;

            std::string name;
            int id;
            std::vector<std::unique_ptr<Object>> objects; 
            SceneNode root;

            bool isInitialized = false;;

            static std::unique_ptr<Scene> createScene(int id, const std::string &name);
            void initScene(bool whichScene, EngineResource::ResourceManager &resourceManagerRef);

            int getId() const {return id;}
            std::string getName() const {return name;}

            void update();

            void cleanupObjects();
    };
}

#endif