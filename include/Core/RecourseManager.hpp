#ifndef _resource_MANAGER_HPP
#define _resource_MANAGER_HPP

#include <type_traits>
#include <any>
#include <unordered_map>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ModelLoader.hpp"
#include "TextureLoader.hpp"

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
                    resource = std::make_shared<resourceType>(TextureLoader::createTexture(filePath));
                } else{
                    static_assert(always_false<resourceType>, "Unsupported resource type!");
                }

                cache[filePath] = resource;
                return resource;
            } 
    };
}

#endif