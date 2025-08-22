#ifndef _MODEL_LOADER_HPP
#define _MODEL_LOADER_HPP

#include "Object.hpp"

using namespace EngineScene;

namespace EngineRenderer{
    class ModelLoader{
        public:
            static Mesh loadModel(std::string meshPath);
    };
}

#endif