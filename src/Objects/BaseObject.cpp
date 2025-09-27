#include "BaseObjects.hpp"
#include "RecourseManager.hpp"
#include "ObjectRegistry.hpp"

namespace EngineObject{
    Object* makeMeshObject(const nlohmann::json& data) {
        MeshObject* meshObject = new MeshObject();
        meshObject->deserialize(data);
        return meshObject;
    }

    static bool registered = []() {
        ObjectRegistry::get().registerType("Mesh_Object", makeMeshObject);
        return true;
    }();

    void MeshObject::deserialize(const nlohmann::json& jsonData){
        pendingMeshPath = jsonData["mesh"];
        pendingTexturePaths.clear();

        for (const auto& texturePath : jsonData["textures"]){
            pendingTexturePaths.push_back(texturePath);
        }
        Shader shader = Shader();
        if(!material) material = std::make_shared<Material>();

        name = jsonData["name"];

        shader.vertShader = jsonData["shader"]["vert"];
        shader.fragShader = jsonData["shader"]["frag"];

        glm::vec4 baseColor;
        baseColor.x = jsonData["base_color"]["r"];
        baseColor.y = jsonData["base_color"]["g"];
        baseColor.z = jsonData["base_color"]["b"];
        baseColor.w = jsonData["base_color"]["a"];

        material->setBaseColor(baseColor);

        material->setRoughness(jsonData["roughness"]);
        material->setMetallic(jsonData["metallic"]);
        material->setAlbedo(jsonData["albedo"]);

        material->setShader(shader);
    }

    MeshObject::MeshObject(glm::vec3 position, const std::string &meshPathRef, const std::string &texturePathRef){
        this->mesh = std::make_shared<Mesh>();
        this->material = std::make_shared<Material>();

        this->type = "Mesh_Object";
        if(!meshPathRef.empty()){
            this->mesh->meshPath = meshPathRef;
        }
        if(!texturePathRef.empty()){
            Texture texture = Texture();
            texture.texturePath = texturePathRef;
            material->addTexture(std::make_shared<Texture>(texture));
        }
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
    }

    void MeshObject::initVulkanResources(EngineResource::ResourceManager &resourceManager){
        if(!pendingMeshPath.empty()){
            mesh = resourceManager.load<Mesh>(pendingMeshPath);
            createBoundingBox();
        } else if(!mesh->meshPath.empty()){
            mesh = resourceManager.load<Mesh>(mesh->meshPath);
            createBoundingBox();
        }

        if(!pendingTexturePaths.empty()){
            material->getTextures().clear();
            for(const auto &texturePath : pendingTexturePaths) {
                if(!texturePath.empty()){
                    auto texture = resourceManager.load<Texture>(texturePath);
                    if(texture){
                        material->addTexture(texture);
                        hasTexture = true;
                    }
                } 
            }
        } else{
            for(auto &texture : material->getTextures()){
                if(texture->texturePath.empty()){
                    continue;
                }
                if(!texture->textureSampler || !texture->textureImage){
                    texture = resourceManager.load<Texture>(texture->texturePath);
                    hasTexture = true;
                }
            }
        }
    }

    void MeshObject::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout){
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &mesh->vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

        vkCmdDrawIndexed(commandBuffer, mesh->indexCount, 1, 0, 0, 0);
    }

    void MeshObject::cleanup(VkDevice device){
        mesh.reset();
        material->getTextures().clear();
    }
}