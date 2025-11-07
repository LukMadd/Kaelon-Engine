#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Object/Mesh.hpp"
#include "Object/Material.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "Object/AABB.hpp"

typedef uint32_t Entity;
constexpr int nullEntity = std::numeric_limits<uint32_t>::max();

using namespace EngineObject;

struct TransformComponent{
    glm::vec3 position{0.0f};
    glm::quat rotation{1,0,0,0};
    glm::vec3 scale{1.0f};

    glm::mat4 modelMatrix{1.0f};
    glm::mat4 localMatrix{1.0f};
    glm::mat4 worldMatrix{1.0f};

    Entity parent = nullEntity;
};

struct MeshComponent{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
};

struct MaterialComponent{
    std::shared_ptr<Material> material;
    bool hasTexture = false;
};

struct PhysicsComponent{
    glm::vec3 velocity{0.0f};
    bool isStatic = false;
};

struct BoundingBoxComponent{
    AABB localBoundingBox = AABB();
    AABB worldBoundingBox = AABB();
};

struct SpatialPartitioningComponent{
    std::vector<uint64_t> cells{};
    bool assignedToCell = false;
};

struct RenderableComponent {
    uint32_t uniformIndex = 0;
};

struct SceneNodeComponent{
    Entity node = nullEntity;
};

struct MetadataComponent{
    std::string name;
    std::string type;
    std::string uuid;
    bool selected = false;
};

#endif