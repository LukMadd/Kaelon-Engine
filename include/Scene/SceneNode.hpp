#include "Transform.hpp"
#include <vector>

using namespace EngineObject;

namespace EngineScene{
    class Object;
    
    class SceneNode{
        public:
            Transform transform;
            Object *object;
            SceneNode *parent;
            std::vector<SceneNode*> children;
    
            void addChild(SceneNode *child);
            void removeChild(SceneNode *child);
            void update(const glm::mat4& parentMatrix = glm::mat4(1.0f));
            glm::vec3 getWorldPosition() const;

    };
}