#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Mesh.hpp"
#include "Material.hpp"
#include "Scene/SceneNode.hpp"
#include "Core/ResourceManager.hpp"
#include "nlohmann/json.hpp"
#include "UUID.hpp"
#include "AABB.hpp"

#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

using namespace EngineRenderer;
using namespace EngineObject;

using namespace EngineScene;

namespace EnginePartitioning{
    class Spacial_Partitioner;
    struct Cell;
}

namespace EngineObject{
    class Object{
        public:
            Object() : uuid(generateUUID()){velocity.y = -1.0f;};
            virtual ~Object() = default;
            virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
            virtual void initResources(EngineResource::ResourceManager &resourceManager, 
                                             EnginePartitioning::Spacial_Partitioner *spacialPartitioner);
            virtual void deserialize(const nlohmann::json& jsonData);
            virtual void cleanup(VkDevice device);
            std::string type;
            std::string name;
            
            uint32_t uniformIndex = 0;

            bool isStatic = false;
            glm::vec3 velocity = glm::vec3(0.0);

            bool assignedToCell = false;

            int objectIndex = 0;
            
            bool selected = false; 
            SceneNode* node = nullptr;

            std::string uuid;
            glm::mat4 modelMatrix{1.0f};

            AABB localBoundingBox = AABB();
            AABB worldBoundingBox = AABB();

            void createBoundingBox();
            void updateCells();
            void move(glm::vec3 position);
            void rotate(glm::vec3 rotation);
            void scale(glm::vec3 scale);

            bool hasTexture = false;

            glm::vec3 getPosition() {return modelMatrix[3];}

            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;

            EnginePartitioning::Spacial_Partitioner *spatialPartitioner;
            std::vector<uint64_t> cells;
    };
}

#endif