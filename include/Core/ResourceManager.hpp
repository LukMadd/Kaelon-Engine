#ifndef _RESOURCE_MANAGER_HPP
#define _RESOURCE_MANAGER_HPP

#include <type_traits>
#include <any>
#include <unordered_map>
#include "Object/Mesh.hpp"
#include "Object/Texture.hpp"
#include "Renderer/ModelLoader.hpp"
#include "Renderer/TextureLoader.hpp"

using namespace EngineObject;

template<class>
inline constexpr bool always_false = false;

namespace EngineResource{
    class ResourceManager{
        std::unordered_map<std::string, std::any> cache;

        public:
            template<typename resourceType>
            std::shared_ptr<resourceType> load(const std::string &filePath){
                auto it = cache.find(filePath);
                if(it != cache.end()){
                    return std::any_cast<std::shared_ptr<resourceType>>(it->second);
                }

                std::shared_ptr<resourceType> resource;

                if constexpr(std::is_same_v<resourceType, Mesh>){
                    resource = std::make_shared<resourceType>(ModelLoader::loadMesh(filePath));
                } else if constexpr(std::is_same_v<resourceType, Texture>){
                    resource = TextureLoader::createTexture(filePath);
                } else{
                    static_assert(always_false<resourceType>, "Unsupported resource type!");
                }

                cache[filePath] = resource;
                return resource;
            } 

            std::unordered_map<std::string, std::any> getCache() {return cache;}

            void cleanup(VkDevice device) {
                for(auto& [path, anyRes] : cache){
                    if(anyRes.type() == typeid(std::shared_ptr<Texture>)){
                        auto tex = std::any_cast<std::shared_ptr<Texture>>(anyRes);
                        if(!tex) continue;

                        if(tex->textureSampler != VK_NULL_HANDLE){
                            vkDestroySampler(device, tex->textureSampler, nullptr);
                            tex->textureSampler = VK_NULL_HANDLE;
                        }
                        if(tex->textureImageView != VK_NULL_HANDLE){
                            vkDestroyImageView(device, tex->textureImageView, nullptr);
                            tex->textureImageView = VK_NULL_HANDLE;
                        }
                        if(tex->textureImage != VK_NULL_HANDLE){
                            vkDestroyImage(device, tex->textureImage, nullptr);
                            tex->textureImage = VK_NULL_HANDLE;
                        }
                        if(tex->textureImageMemory != VK_NULL_HANDLE){
                            vkFreeMemory(device, tex->textureImageMemory, nullptr);
                            tex->textureImageMemory = VK_NULL_HANDLE;
                        }
                    } 
                    else if(anyRes.type() == typeid(std::shared_ptr<Mesh>)){
                        auto mesh = std::any_cast<std::shared_ptr<Mesh>>(anyRes);
                        if(!mesh) continue;

                        if(mesh->vertexBuffer.buffer != VK_NULL_HANDLE){
                            vkDestroyBuffer(device, mesh->vertexBuffer.buffer, nullptr);
                            mesh->vertexBuffer.buffer = VK_NULL_HANDLE;
                        }
                        if(mesh->vertexBuffer.bufferMemory != VK_NULL_HANDLE){
                            vkFreeMemory(device, mesh->vertexBuffer.bufferMemory, nullptr);
                            mesh->vertexBuffer.bufferMemory = VK_NULL_HANDLE;
                        }
                        if(mesh->indexBuffer.buffer != VK_NULL_HANDLE){
                            vkDestroyBuffer(device, mesh->indexBuffer.buffer, nullptr);
                            mesh->indexBuffer.buffer = VK_NULL_HANDLE;
                        }
                        if(mesh->indexBuffer.bufferMemory != VK_NULL_HANDLE){
                            vkFreeMemory(device, mesh->indexBuffer.bufferMemory, nullptr);
                            mesh->indexBuffer.bufferMemory = VK_NULL_HANDLE;
                        }
                    }
                }

                cache.clear();
            }


    };
}

#endif