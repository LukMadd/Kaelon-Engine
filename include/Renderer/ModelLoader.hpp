#ifndef _MODEL_LOADER_HPP
#define _MODEL_LOADER_HPP

#include "Object/Mesh.hpp"

using namespace EngineObject;

namespace EngineRenderer{
    class ModelLoader{
        public:
            static Mesh loadMesh(std::string meshPath);
    };
}

#endif