#include "Camera/Camera.hpp"
#include "ECS/Components.hpp"
#include "Scene/Scene.hpp"
#include "nlohmann/json.hpp"

using namespace nlohmann;
;
using namespace EngineScene;
using namespace EngineCamera;

json serializeEntityData(const Entity e, ECS& ecs);
json serialzeCamera(std::shared_ptr<EngineCamera::Camera> Camera);
Entity deserializeEntity(const nlohmann::json& jsonData, ECS& ecs);
SceneNodeComponent* deserializeNode(Scene &scene, const json& jsonNode, ECS& ecs);
Camera* deserializeCamera(const nlohmann::json& jsonData, ECS& ecs);
