#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Mesh.hpp"
#include "Material.hpp"
#include "Scene/SceneNode.hpp"
#include "Core/RecourseManager.hpp"
#include "nlohmann/json.hpp"
#include "UUID.hpp"

#include <cstddef>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

using namespace EngineRenderer;
using namespace EngineObject;

struct AAB{
    glm::vec3 min = glm::vec3(FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX);

    bool isInitialized = false;
};

using namespace EngineScene;

namespace EnginePartitioning{
    class Spacial_Partitioner;
}

namespace EngineObject{
    class Object{
        public:
            Object() : uuid(generateUUID()){};
            virtual ~Object() = default;
            virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) = 0;
            virtual void initVulkanResources(EngineResource::ResourceManager &resourceManager, 
                                             EnginePartitioning::Spacial_Partitioner *spacialPartitioner) = 0;
            virtual void deserialize(const nlohmann::json& jsonData) = 0;
            virtual void cleanup(VkDevice device) = 0;
            std::string type;
            std::string name;
            
            uint32_t uniformIndex = 0;

            bool isStatic = false;
            bool isStoppedByGround = false;
            glm::vec3 velocity = glm::vec3(0.0);

            int objectIndex = 0;
            
            bool selected = false; 
            SceneNode* node = nullptr;

            std::string uuid;
            glm::mat4 modelMatrix{1.0f};

            AAB localBoundingBox = AAB();
            AAB worldBoundingBox = AAB();

            void createBoundingBox();
            void move(glm::vec3 position);
            void rotate(glm::vec3 rotation);
            void scale(glm::vec3 scale);

            bool hasTexture = false;

            glm::vec3 getPosition() {return modelMatrix[3];}

            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;

            EnginePartitioning::Spacial_Partitioner *spatialPartitioner;

    };
}

#endif