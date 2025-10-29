
#include <cstdint>
#include "Object/Object.hpp"
#include "Scene/SceneManager.hpp"
#include "Spatial/Spatial_Partitioner.hpp"

namespace EnginePhysics{
    class PhysicsEngine{
        public:
            void init(SceneManager *sceneManager, 
                      EnginePartitioning::Spacial_Partitioner *spatialPartitioner);

            void tick(float deltaTime);

            bool checkAABBCollision(const AABB &a, const AABB &b);

            int checkAABBObjectCollisionYAxis(const AABB &a, const AABB &b, float previousMinY, 
                                            float previousMaxY, float epsilon);

            int checkAABBObjectCollisionXAxis(const AABB &a, const AABB &b, float previousMinX, 
                                            float previousMaxX, float epsilon);

            float getTickRate() {return tickRate;}

            uint64_t getTickCount() {return tickCount;}

        private:
            uint64_t currentTick;

            const float tickRate = 16.66666667 / 1000.0f; //60 updates a second

            const glm::vec3 gravity = glm::vec3(0.0, -9.81, 0.0); //Goes down 1/10 unit a tick

            uint64_t tickCount = 0;

            SceneManager *sceneManager;

            EnginePartitioning::Spacial_Partitioner *spatialPartitioner;
    };
}