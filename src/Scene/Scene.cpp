#include "Scene.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace EngineScene{
    void Scene::initScene(){
            auto quad1 = std::make_unique<Quad>(glm::vec3(0.0f, 0.0f, 0.0f));

            auto quad2 = std::make_unique<Quad>(glm::vec3(0.0f, 1.0f, 0.0f));

            objects.push_back(std::move(quad1));
            objects.push_back(std::move(quad2));
    }

    void Scene::cleanup(){
        for(int i = 0; i < objects.size(); i++){
            if(objects[i]->mesh->vertexBuffer.buffer){
                vkDestroyBuffer(device, objects[i]->mesh->vertexBuffer.buffer, nullptr);
            }
            if(objects[i]->mesh->vertexBuffer.bufferMemory){
                vkFreeMemory(device, objects[i]->mesh->vertexBuffer.bufferMemory, nullptr);
            }
            if(objects[i]->mesh->indexBuffer.buffer){
                vkDestroyBuffer(device, objects[i]->mesh->indexBuffer.buffer, nullptr);
            }
            if(objects[i]->mesh->indexBuffer.bufferMemory){
                vkFreeMemory(device, objects[i]->mesh->indexBuffer.bufferMemory, nullptr);
            }
        }
    }
}