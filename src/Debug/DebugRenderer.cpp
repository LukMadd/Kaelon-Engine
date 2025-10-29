#include "Debug/DebugRenderer.hpp"
#include "Renderer/Vertex.hpp"
#include "Renderer/RendererGlobals.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

constexpr float BOUNDING_BOX_LINE_WIDTH = 0.20f;

using namespace EngineRenderer;

void DebugRenderer::addLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width, float angle){
    debugLines.push_back({start, end, color, width, angle});
}

void DebugRenderer::setupLines(VertexBuffer &vertexBuffer, IndexBuffer &indexBuffer, const glm::mat4 &view, const glm::vec3 &front){
    if(vertexBuffer.buffer){
        vkDestroyBuffer(device, vertexBuffer.buffer, nullptr);
    }
    if(vertexBuffer.bufferMemory){
        vkFreeMemory(device, vertexBuffer.bufferMemory, nullptr);
    }
    if(indexBuffer.buffer){
        vkDestroyBuffer(device, indexBuffer.buffer, nullptr);
    }
    if(indexBuffer.bufferMemory){
        vkFreeMemory(device, indexBuffer.bufferMemory, nullptr);
    }

    if(debugLines.empty()) return;

    uint32_t idx = 0;

    glm::vec3 camDir = glm::normalize(front);

    for(const auto &line : debugLines){
        glm::vec3 lineDir = glm::normalize(line.end - line.start);
        glm::vec3 viewDir = glm::normalize(front);
        glm::vec3 up = {0, 1, 0};
        if(fabs(glm::dot(lineDir, up)) > 0.99f){
            up = glm::vec3(1, 0, 0);
        }

        glm::vec3 perp = glm::normalize(glm::cross(lineDir, up));
        glm::vec3 second = glm::normalize(glm::cross(lineDir, perp));

        float angle = glm::radians(line.angle);
        glm::vec3 rotatedPerp = perp * cos(angle) + second * sin(angle);        
        
        perp = rotatedPerp * (line.width * 0.5f);

        glm::vec3 v0 = line.start + perp;
        glm::vec3 v1 = line.end   + perp;
        glm::vec3 v2 = line.end   - perp;
        glm::vec3 v3 = line.start - perp;

        glm::vec3 normal = glm::vec3(0.0f);

        vertexBuffer.vertices.push_back({v0, normal, line.color, glm::vec2(0.0f, 1.0f)});
        vertexBuffer.vertices.push_back({v1, normal, line.color, glm::vec2(1.0f, 1.0f)});
        vertexBuffer.vertices.push_back({v2, normal, line.color, glm::vec2(1.0f, 0.0f)});
        vertexBuffer.vertices.push_back({v3, normal, line.color, glm::vec2(0.0f, 0.0f)});

        indexBuffer.indices.push_back(idx + 0);
        indexBuffer.indices.push_back(idx + 1);
        indexBuffer.indices.push_back(idx + 2);
        indexBuffer.indices.push_back(idx + 2);
        indexBuffer.indices.push_back(idx + 3);
        indexBuffer.indices.push_back(idx + 0);

        idx += 4;
    }

    vertexBuffer.createVertexBuffer();
    indexBuffer.createIndexBuffer();

    debugLines.clear();
}

void DebugRenderer::drawBoundingBox(AABB &worldBoundingBox){
    float offset = (BOUNDING_BOX_LINE_WIDTH / 25.0f); //To prevent the line from be taken out by depth testing
    const float BOUNDING_BOX_MAX_X = worldBoundingBox.max.x + offset;
    const float BOUNDING_BOX_MAX_Y = worldBoundingBox.max.y + offset;
    const float BOUNDING_BOX_MAX_Z = worldBoundingBox.max.z + offset;
    const float BOUNDING_BOX_MIN_X = worldBoundingBox.min.x - offset;
    const float BOUNDING_BOX_MIN_Y = worldBoundingBox.min.y - offset;
    const float BOUNDING_BOX_MIN_Z = worldBoundingBox.min.z - offset;

    glm::vec3 corners[8] = {
        {BOUNDING_BOX_MIN_X, BOUNDING_BOX_MIN_Y, BOUNDING_BOX_MIN_Z}, //0: bottom-left-front
        {BOUNDING_BOX_MAX_X, BOUNDING_BOX_MIN_Y, BOUNDING_BOX_MIN_Z}, //1: bottom-right-front
        {BOUNDING_BOX_MAX_X, BOUNDING_BOX_MAX_Y, BOUNDING_BOX_MIN_Z}, //2: top-right-front
        {BOUNDING_BOX_MIN_X, BOUNDING_BOX_MAX_Y, BOUNDING_BOX_MIN_Z}, //3: top-left-front
        {BOUNDING_BOX_MIN_X, BOUNDING_BOX_MIN_Y, BOUNDING_BOX_MAX_Z}, //4: bottom-left-back
        {BOUNDING_BOX_MAX_X, BOUNDING_BOX_MIN_Y, BOUNDING_BOX_MAX_Z}, //5: bottom-right-back
        {BOUNDING_BOX_MAX_X, BOUNDING_BOX_MAX_Y, BOUNDING_BOX_MAX_Z}, //6: top-right-back
        {BOUNDING_BOX_MIN_X, BOUNDING_BOX_MAX_Y, BOUNDING_BOX_MAX_Z}  //7: top-left-back
    };

    const float BOUNDING_BOX_X_MODIFIER = (BOUNDING_BOX_MAX_X - BOUNDING_BOX_MIN_X)/10;
    const float BOUNDING_BOX_Y_MODIFIER = (BOUNDING_BOX_MAX_Y - BOUNDING_BOX_MIN_Y)/10;
    const float BOUNDING_BOX_Z_MODIFIER = (BOUNDING_BOX_MAX_Z - BOUNDING_BOX_MIN_Z)/10;

    const float GENERAL_MODIFIER = (BOUNDING_BOX_X_MODIFIER + BOUNDING_BOX_Y_MODIFIER + BOUNDING_BOX_Z_MODIFIER) / 3;

    const glm::vec3 color = {1.0f, 0.0f, 0.0f};

    float angle = 0.0f;
    float width = BOUNDING_BOX_LINE_WIDTH * (GENERAL_MODIFIER / 4);;

    glm::vec3 start;
    glm::vec3 end;
    
    for(int i = 0; i < sizeof(corners) / sizeof(corners[0]); i++){
        for(int j = 0; j < sizeof(corners) / sizeof(corners[0]); j++){
            if(i == j) continue;

            start = corners[i];
            end = corners[j];

            bool isSameXAxis = corners[i].x - corners[j].x == 0.0f;
            bool isSameYAxis = corners[i].y - corners[j].y == 0.0f;
            bool isSameZAxis = corners[i].z - corners[j].z == 0.0f;

            int sameAxisesCount = isSameXAxis + isSameYAxis + isSameZAxis;
            std::cout << "SAME AXIS: " << sameAxisesCount << std::endl;

            if(sameAxisesCount < 1){
                continue;
            }

            if(isSameYAxis == false){
                angle = 90.0f;
            }

            addLine(start, end, color, width, angle);
        }
    }
}
