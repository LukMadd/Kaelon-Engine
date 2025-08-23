#include "RecourseManager.hpp"
#include "ModelLoader.hpp"
#include "TextureLoader.hpp"

using namespace EngineRenderer;

namespace EngineRecourse{
    std::shared_ptr<Mesh> RecourseManager::loadMesh(const std::string &filePath){
        auto it = meshCache.find(filePath);
        if(it != meshCache.end()){
            return it->second;
        }

        auto mesh = ModelLoader::loadMesh(filePath);    
        auto meshPtr = std::make_shared<Mesh>(mesh); 
        meshCache[filePath] = meshPtr;
        return meshPtr;
    }

    std::shared_ptr<Texture> RecourseManager::loadTexture(const std::string &filePath){
        auto it = textureCache.find(filePath);
        if(it != textureCache.end()){
            return it->second;
        }

        auto texture = TextureLoader::createTexture(filePath);
        auto texturePtr = std::make_shared<Texture>(texture);
        textureCache[filePath] = texturePtr;
        return texturePtr;
    }
}