#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Object.hpp"
#include "RecourseManager.hpp"
#include "SceneNode.hpp"

namespace EngineScene{
    class Scene{
        public:
            Scene();
            std::vector<std::unique_ptr<Object>> objects; 
            SceneNode root;

            bool isInitialized = false;;

            static Scene createScene();
            void initScene(bool whichScene, EngineRecourse::RecourseManager &recourseManagerRef);

            void update();

            void cleanupObjects();
    };
}

#endif