#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace EngineObject;
using namespace EngineScene;
using namespace EngineCamera;

json serializeObject(const EngineObject::Object &object);
json serializeNode(SceneNode *node);
json serialzeCamera(std::shared_ptr<EngineCamera::Camera> Camera);
EngineObject::Object* deserializeObject(const nlohmann::json& jsonData);
SceneNode* deserializeNode(Scene &scene, const json& jsonNode);
Camera* deserializeCamera(const nlohmann::json& jsonData);
