#include "Scene.hpp"
#include "Cube.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineObject;

namespace EngineScene{
    void Scene::initScene(){
            glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);

            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    auto cube = std::make_unique<Cube>(pos, "models/Crate1.obj", "textures/crate_1.jpg");
                    cube->modelMatrix = glm::scale(cube->modelMatrix, glm::vec3(0.5f));

                    objects.push_back(std::move(cube));

                    pos.x+=1.75;
                }
                pos.x = 0;
                pos.z+=1.5;
                pos.y+=1.5;
        }
    }

    void Scene::cleanupObjects(){
        for (auto& obj : objects) {
            if (obj->mesh.vertexBuffer.buffer) {
                vkDestroyBuffer(device, obj->mesh.vertexBuffer.buffer, nullptr);
                obj->mesh.vertexBuffer.buffer = VK_NULL_HANDLE;
            }
            if (obj->mesh.vertexBuffer.bufferMemory) {
                vkFreeMemory(device, obj->mesh.vertexBuffer.bufferMemory, nullptr);
                obj->mesh.vertexBuffer.bufferMemory = VK_NULL_HANDLE;
            }
            if (obj->mesh.indexBuffer.buffer) {
                vkDestroyBuffer(device, obj->mesh.indexBuffer.buffer, nullptr);
                obj->mesh.indexBuffer.buffer = VK_NULL_HANDLE;
            }
            if (obj->mesh.indexBuffer.bufferMemory) {
                vkFreeMemory(device, obj->mesh.indexBuffer.bufferMemory, nullptr);
                obj->mesh.indexBuffer.bufferMemory = VK_NULL_HANDLE;
            }
            if (obj->texture.textureSampler) {
                vkDestroySampler(device, obj->texture.textureSampler, nullptr);
                obj->texture.textureSampler = VK_NULL_HANDLE;
            }
            if (obj->texture.textureImageView) {
                vkDestroyImageView(device, obj->texture.textureImageView, nullptr);
                obj->texture.textureImageView = VK_NULL_HANDLE;
            }
            if (obj->texture.textureImage) {
                vkDestroyImage(device, obj->texture.textureImage, nullptr);
                obj->texture.textureImage = VK_NULL_HANDLE;
            }
            if (obj->texture.textureImageMemory) {
                vkFreeMemory(device, obj->texture.textureImageMemory, nullptr);
                obj->texture.textureImageMemory = VK_NULL_HANDLE;
            }
        }
        objects.clear();
    }
}