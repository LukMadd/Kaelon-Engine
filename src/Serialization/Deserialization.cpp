#include "Serialization/Serialization.hpp"
#include "Core/ObjectRegistry.hpp"

Object* deserializeObject(const nlohmann::json& objectJson){
    Object* obj = ObjectRegistry::get().create(objectJson["type"], objectJson);
    obj->uuid = objectJson.value("uuid", obj->uuid);
    obj->isStatic = objectJson["is_static"];

    return obj;
}

SceneNode* deserializeNode(Scene &scene, const json& jsonNode){
    SceneNode *node = new SceneNode();
    node->object = nullptr;

    if(jsonNode.contains("transform")){
        const auto &transform = jsonNode["transform"];
        if(transform.contains("position") && transform["position"].size() >= 3)
            node->transform.position = glm::vec3(transform["position"][0], transform["position"][1], transform["position"][2]);
        if(transform.contains("rotation") && transform["rotation"].size() >= 4)
            node->transform.rotation = glm::quat(transform["rotation"][3], transform["rotation"][0], transform["rotation"][1], transform["rotation"][2]); // w,x,y,z
        if(transform.contains("scale") && transform["scale"].size() >= 3)
            node->transform.scale = glm::vec3(transform["scale"][0], transform["scale"][1], transform["scale"][2]);
    }

    if(jsonNode.contains("object")){
        const json &jsonData = jsonNode["object"];
        Object* object = deserializeObject(jsonData);
        object->node = node;
        scene.objects.push_back(std::unique_ptr<Object>(object));
        node->object = scene.objects.back().get();
    }

    if(jsonNode.contains("children")){
        for(auto& childJson : jsonNode["children"]){
            SceneNode* childNode = deserializeNode(scene, childJson);
            node->addChild(childNode);
        }
    }

    return node;
}

Camera* deserializeCamera(const nlohmann::json& jsonData){
    EngineCamera::Camera* camera = new EngineCamera::Camera();

    camera->is_initialized = jsonData["is_initialized"];

    camera->name = jsonData["name"];
    
    camera->getFov() = jsonData["fov"];
    
    camera->getFront().x = jsonData["front"][0];
    camera->getFront().y = jsonData["front"][1];
    camera->getFront().z = jsonData["front"][2];

    camera->getUp().x = jsonData["up"][0];
    camera->getUp().y = jsonData["up"][1];
    camera->getUp().z = jsonData["up"][2];

    camera->position.x = jsonData["position"][0];
    camera->position.y = jsonData["position"][1];
    camera->position.z = jsonData["position"][2];

    camera->getYaw() = jsonData["rotation"]["yaw"];
    camera->getPitch() = jsonData["rotation"]["pitch"];

    camera->getSensitivity() = jsonData["sensitivity"];

    camera->getSpeed() = jsonData["speed"];

    return camera;
}