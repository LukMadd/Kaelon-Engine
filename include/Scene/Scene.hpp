#include "Object.hpp"

namespace EngineScene{
    class Scene{
        public:
            std::vector<std::unique_ptr<Object>> objects; 

            void initScene();

            void cleanupObjects();
    };
}