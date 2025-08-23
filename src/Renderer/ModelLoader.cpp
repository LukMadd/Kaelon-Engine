#include "ModelLoader.hpp"
#include "Vertex.hpp"
#include "RendererGlobals.hpp"

#include <cstdint>
#include <stdexcept>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace EngineRenderer{
    Mesh ModelLoader::loadMesh(std::string meshPath){
        tinyobj::ObjReaderConfig reader_config;
        reader_config.triangulate = true;

        tinyobj::ObjReader reader;
        if (!reader.ParseFromFile("../" + meshPath, reader_config)) {
            throw std::runtime_error(reader.Error());
        }

        tinyobj::attrib_t attrib = reader.GetAttrib();
        std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
        std::vector<tinyobj::material_t> materials = reader.GetMaterials();
        std::string warn, err;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for(const auto& shape : shapes){
            for(const auto& index : shape.mesh.indices){
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                 if(!attrib.texcoords.empty() && index.texcoord_index >= 0){
                    vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }else {
                    vertex.texCoord = {0.0f, 0.0f};
                }

                vertex.color = {1.0f, 1.0f, 1.0f};

                if(uniqueVertices.count(vertex) ==0){
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
        Mesh mesh;
        mesh.vertexBuffer.vertices = vertices;
        mesh.indexBuffer.indices = indices;
        mesh.indexCount = static_cast<uint32_t>(indices.size());
        mesh.meshPath = meshPath;
        mesh.vertexBuffer.createVertexBuffer(vertices, mesh.vertexBuffer.buffer, mesh.vertexBuffer.bufferMemory);
        mesh.indexBuffer.createIndexBuffer(indices, mesh.indexBuffer.buffer, mesh.indexBuffer.bufferMemory);

        return mesh;

    }
}