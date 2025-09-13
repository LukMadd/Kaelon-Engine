#include "SceneManager.hpp"
#include <filesystem>
#include <fstream>
#include <memory>

#include "ObjectRegistry.hpp"
#include "RecourseManager.hpp"
#include "nlohmann/json.hpp"

using namespace EngineObject;

namespace EngineScene{
    SceneManager::SceneManager() : currentID(0), currentSceneIndex(0), resourceManager(nullptr){};

    void SceneManager::addScene(const std::string &name, int id){
        auto scene = Scene::createScene(id, name);
        scene->initScene(false, *resourceManager);
        sceneOrder.push_back(id); //Pushes the scenes ID into sceneOrder to be used for current scene tracking
        scenes[id] = std::move(scene);

        currentID++;
    }

    void SceneManager::changeScenes(int indexChange){
        int newIndex = currentSceneIndex + indexChange;
        if(newIndex >= sceneOrder.size() || newIndex < 0) return;

        int newSceneID = sceneOrder[newIndex];
        auto &scenePtr = scenes.at(newSceneID);

        if(!scenePtr->isInitialized){
            scenePtr->initScene(false, *resourceManager);
        }

        currentSceneIndex = newIndex;
    }

    void SceneManager::init(EngineResource::ResourceManager &resourceManagerRef){
        resourceManager = &resourceManagerRef;

        std::vector<std::filesystem::path> files;
        for (auto& entry : std::filesystem::directory_iterator("../scenes"))
            if (std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".json")
                files.push_back(entry.path());

        std::sort(files.begin(), files.end());     
        
        if(files.empty()){
            addScene("Base Scene", currentID);
        } else{
            for(auto &file : files){
                deserializeScene(file.string());
            }
        }
        
        if(!sceneOrder.empty()) currentSceneIndex = 0; 
    }

    json SceneManager::serializeObject(const Object &object){
        json jsonData;
        if(!object.name.empty()){
            jsonData["name"] = object.name;
        } else{
            jsonData["name"] = "Generic";
        }
        jsonData["uuid"] = object.uuid;
        jsonData["type"] = object.type;
        jsonData["mesh"] = object.mesh->meshPath;
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

    json SceneManager::serializeNode(SceneNode* node){
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
    
    Object* SceneManager::deserializeObject(const nlohmann::json& objectJson){
        Object* obj = ObjectRegistry::get().create(objectJson["type"], objectJson);
        obj->uuid = objectJson.value("uuid", obj->uuid);

        return obj;
    }

    void SceneManager::serializeScene(Scene &scene, uint32_t sceneIndex){
        json sceneData;
        sceneData["name"] = scene.name;
        sceneData["id"] = scene.id;
        sceneData["objects"] = json::array();

        //Loops through the scenes children and serializes them
        for(auto &node : scene.root.children){
            sceneData["objects"].push_back(serializeNode(node));
        }

        std::filesystem::create_directories(("../scenes"));
        std::ofstream file("../scenes/scene" + std::to_string(sceneIndex) + ".json");
        if(!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file");
        }

        file << sceneData.dump(4);
        file.close();
    }

    SceneNode* SceneManager::deserializeNode(Scene &scene, const json& jsonNode){
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


    void SceneManager::deserializeScene(const std::string& filename){
        std::ifstream file(filename);
        if(!file.is_open()) throw std::runtime_error("Failed to open file " + filename + " for deseralization");

        json sceneData;
        file >> sceneData;

        std::unique_ptr<Scene> scene = Scene::createScene(sceneData["id"], sceneData["name"]);

        scene->objects.clear();

        for(auto &jsonObj : sceneData["objects"]){
            SceneNode* node = deserializeNode(*scene, jsonObj);
            scene->root.addChild(node);
        }

        sceneOrder.push_back(scene->id);

        scenes[scene->id] = (std::move(scene));

        currentID++;
    }

    void SceneManager::saveScenes(){
        for(auto& [id, scene] : scenes){
            serializeScene(*scene, id);
        }
    }

     std::vector<Scene*> SceneManager::getScenes(){
        std::vector<Scene*> scenesVector;

        for(auto &scene : scenes){
            scenesVector.push_back(scene.second.get());
        }

        return scenesVector;
    }

    Scene* SceneManager::getCurrentScene(){
        return scenes.at(sceneOrder[currentSceneIndex]).get();
    }

    Scene* SceneManager::getScene(int id){
        auto it = scenes.find(id);
        if(it != scenes.end()) return it->second.get();
        return nullptr;
    }
}