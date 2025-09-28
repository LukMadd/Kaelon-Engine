#include "Serialization/Serialization.hpp"
#include "Object/Object.hpp"

json serializeObject(const EngineObject::Object &object){
    json jsonData;
    if(!object.name.empty()){
        jsonData["name"] = object.name;
    } else{
        jsonData["name"] = "Generic";
     }

    jsonData["uuid"] = object.uuid;
    jsonData["type"] = object.type;
    jsonData["mesh"] = object.mesh->meshPath;
    jsonData["is_static"] = object.isStatic; 
        jsonData["base_color"] = {
            {"r", object.material->getBaseColor().x},
            {"g", object.material->getBaseColor().y},
            {"b", object.material->getBaseColor().z},
            {"a", object.material->getBaseColor().w},
        };
        jsonData["roughness"] = object.material->getRoughness();
        jsonData["metallic"] = object.material->getMetallic();
        jsonData["albedo"] = object.material->getAlbedo();

    jsonData["textures"] = json::array();
    for(const auto &texture : object.material->getTextures()){
        jsonData["textures"].push_back(texture->texturePath);
    }
    jsonData["shader"] = {
        {"vert", object.material->getShader().vertShader},
        {"frag", object.material->getShader().fragShader}
    };

    return jsonData; 
}

json serializeNode(SceneNode* node){
    json jsonData = json::object();

    jsonData["transform"] = {
        {"position",{node->transform.position.x, node->transform.position.y, node->transform.position.z }},
        {"rotation",{node->transform.rotation.x, node->transform.rotation.y, node->transform.rotation.z, node->transform.rotation.w }}, // x,y,z,w
        {"scale", {node->transform.scale.x,    node->transform.scale.y,    node->transform.scale.z }}
    };

    if(node->object){
        jsonData["object"] = serializeObject(*node->object);
    }

    if(!node->children.empty()){
        jsonData["children"] = json::array();
        for(auto &child : node->children){
            jsonData["children"].push_back(serializeNode(child));
        }
    }
    return jsonData;
}

json serialzeCamera(std::shared_ptr<EngineCamera::Camera> camera){
    if(!camera->is_initialized){
        return nullptr;
    }

    json cameraData = json::object();

    cameraData["is_initialized"] = camera->is_initialized;

    cameraData["name"] = camera->name;

    cameraData["position"] = {
        camera->position.x, camera->position.y, camera->position.z
    };

    glm::vec3 front = camera->getFront();
    glm::vec3 up = camera->getUp();

    cameraData["front"] = {
        front.x, front.y, front.z
    };

    cameraData["up"] = {
        up.x, up.y, up.z,
    };

    cameraData["fov"] = camera->getFov();
    cameraData["speed"] = camera->getSpeed();
    
    cameraData["rotation"] = {
        {"yaw", camera->getYaw()},
        {"pitch", camera->getPitch()}
    };

    cameraData["sensitivity"] = camera->getSensitivity();

    return cameraData;
}
