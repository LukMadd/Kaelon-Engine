#ifndef _MESH_HPP
#define _MESH_HPP

#include "Renderer/Vertex.hpp"

using namespace EngineRenderer;

struct Mesh{
    std::string meshPath;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    uint32_t indexCount;
};


#endif