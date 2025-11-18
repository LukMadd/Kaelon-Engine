#ifndef DEBUG_RENDERER_HPP
#define DEBUG_RENDERER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "Renderer//Vertex.hpp"
#include "Scene/SceneManager.hpp"

struct DebugLine3D{
    glm::vec3 start;
    glm::vec3 end;
    glm::vec3 color;
    float width;
    float angle;
};

struct Matricies{
    glm::mat4 proj;
    glm::mat4 view;
};

using namespace EngineRenderer;

class DebugRenderer{
    public:
        std::vector<DebugLine3D> debugLines;

        void addLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width = 0.0f, float angle = 0.0f);

        void pushLine(DebugLine3D &line){debugLines.push_back(line);}

        void setupLines(VertexBuffer &vertexBuffer, IndexBuffer &indexBufferfloat, const glm::mat4 &view, const glm::vec3 &front);

        void drawBoundingBox(AABB &worldBoundingBox);

        void cleanup();

    private:
        EngineScene::SceneManager *sceneManager = nullptr;;
};

#endif