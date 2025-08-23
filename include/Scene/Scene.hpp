#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Object.hpp"
#include "RecourseManager.hpp"

namespace EngineScene{
    class Scene{
        public:
            std::vector<std::unique_ptr<Object>> objects; 

            bool isInitialized = false;;

            static Scene createScene();
            void initScene(bool whichScene, EngineRecourse::RecourseManager &recourseManagerRef);

            void cleanupObjects();
    };
}

#endif