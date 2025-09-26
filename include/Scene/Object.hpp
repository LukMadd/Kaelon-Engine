#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include "Mesh.hpp"
#include "Material.hpp"
#include "SceneNode.hpp"
#include "nlohmann/json.hpp"
#include "RecourseManager.hpp"
#include "UUID.hpp"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

using namespace EngineRenderer;
using namespace EngineObject;

struct AAB{
    glm::vec3 min = glm::vec3(FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX);
};

namespace EngineScene{
    class Object{
        public:
            Object() : uuid(generateUUID()){};
            virtual ~Object() = default;
            virtual void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) = 0;
            virtual void initVulkanResources(EngineResource::ResourceManager &resourceManager) = 0;
            virtual void deserialize(const nlohmann::json& jsonData) = 0;
            virtual void cleanup(VkDevice device) = 0;
            std::string type;
            std::string name;
            
            uint32_t uniformIndex = 0;

            int objectIndex = 0;
            
            bool selected = false; 
            SceneNode* node = nullptr;

            std::string uuid;
            glm::mat4 modelMatrix{1.0f};

            AAB localBoundingBox;
            AAB worldBoundingBox;

            bool hasTexture = false;

            glm::vec3 getPosition() {return modelMatrix[3];}

            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
    };
}

#endif