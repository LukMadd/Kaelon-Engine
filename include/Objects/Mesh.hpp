#ifndef _MESH_HPP
#define _MESH_HPP

#include "Vertex.hpp"

using namespace EngineRenderer;

namespace EngineObject{
    struct Mesh{
        std::string meshPath;
        VertexBuffer vertexBuffer;
        IndexBuffer indexBuffer;
        uint32_t indexCount;
    };
}

#endif