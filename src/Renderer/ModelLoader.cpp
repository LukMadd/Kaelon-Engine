#include "ModelLoader.hpp"
#include "Vertex.hpp"
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>

namespace EngineRenderer{

struct VertexHash{
    size_t operator()(const Vertex& v) const{
        size_t h1 = std::hash<glm::vec3>()(v.pos);
        size_t h2 = std::hash<glm::vec2>()(v.texCoord);
        return h1 ^ (h2 << 1);
    }
};

Mesh ModelLoader::loadMesh(std::string meshPath){
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;

    tinyobj::ObjReader reader;
    if(!reader.ParseFromFile(std::string(KAELON_MODEL_DIR) + meshPath, reader_config)){
        throw std::runtime_error(reader.Error());
    }

    tinyobj::attrib_t attrib = reader.GetAttrib();
    std::vector<tinyobj::shape_t> shapes = reader.GetShapes();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    bool hasNormals = !attrib.normals.empty();
    std::unordered_map<Vertex,uint32_t,VertexHash> uniqueVertices;

    for(const auto& shape:shapes){
        for(const auto& index:shape.mesh.indices){
            Vertex vertex{};
            vertex.pos = {
                attrib.vertices[3*index.vertex_index+0],
                attrib.vertices[3*index.vertex_index+1],
                attrib.vertices[3*index.vertex_index+2]
            };
            vertex.color = {1.0f,1.0f,1.0f};
            if(!attrib.texcoords.empty() && index.texcoord_index>=0){
                vertex.texCoord = {
                    attrib.texcoords[2*index.texcoord_index+0],
                    1.0f- attrib.texcoords[2*index.texcoord_index+1]
                };
            } else{
                vertex.texCoord = {0.0f,0.0f};
            }
            vertex.normal = hasNormals && index.normal_index>=0 ? glm::vec3{
                attrib.normals[3*index.normal_index+0],
                attrib.normals[3*index.normal_index+1],
                attrib.normals[3*index.normal_index+2]
            } : glm::vec3(0.0f);

            if(uniqueVertices.count(vertex)==0){
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }

    if(!hasNormals){
        std::vector<glm::vec3> normalAcc(vertices.size(), glm::vec3(0.0f));
        for(size_t i=0;i<indices.size();i+=3){
            uint32_t i0 = indices[i+0];
            uint32_t i1 = indices[i+1];
            uint32_t i2 = indices[i+2];
            glm::vec3 edge1 = vertices[i1].pos - vertices[i0].pos;
            glm::vec3 edge2 = vertices[i2].pos - vertices[i0].pos;
            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1,edge2));
            normalAcc[i0] += faceNormal;
            normalAcc[i1] += faceNormal;
            normalAcc[i2] += faceNormal;
        }
        for(size_t i=0;i<vertices.size();++i){
            vertices[i].normal = glm::normalize(normalAcc[i]);
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