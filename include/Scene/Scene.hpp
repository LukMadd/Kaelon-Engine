#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "Object.hpp"

namespace EngineScene{
    class Scene{
        public:
            std::vector<std::unique_ptr<Object>> objects; 

            bool isInitialized = false;;

            static Scene createScene();
            void initScene(bool whichScene);

            void cleanupObjects();
    };
}

#endif