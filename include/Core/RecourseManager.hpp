#ifndef _RECOURSE_MANAGER_HPP
#define _RECOURSE_MANAGER_HPP

#include <unordered_map>
#include "Mesh.hpp"
#include "Texture.hpp"

using namespace EngineObject;


namespace EngineRecourse{
    class RecourseManager{
        std::unordered_map<std::string, std::shared_ptr<Mesh>> meshCache;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;

        public:
            std::shared_ptr<Mesh> loadMesh(const std::string &filePath);
            std::shared_ptr<Texture> loadTexture(const std::string &filePath);
    };
}

#endif