#include "SceneManager.hpp"
#include <filesystem>
#include <fstream>
#include <memory>

#include "Camera.hpp"
#include "ObjectRegistry.hpp"
#include "RecourseManager.hpp"
#include "nlohmann/json.hpp"

#include "Serialization.hpp"

using namespace EngineObject;
using namespace nlohmann;


namespace EngineScene{
    SceneManager::SceneManager() : currentID(0), currentSceneIndex(0), resourceManager(nullptr){};

    void SceneManager::addDefaultScene(){
        auto scene = Scene::createScene(currentID, "Default_Scene");
        scene->initBaseScene(*resourceManager);
        sceneOrder.push_back(currentID); //Pushes the scenes ID into sceneOrder to be used for current scene tracking
        scene->update();
        scenes[currentID] = std::move(scene);

        currentID++;
    }

    void SceneManager::changeScenes(int index){
        if(index >= sceneOrder.size() || index < 0) return;

        int newSceneID = sceneOrder[index];
        auto &scenePtr = scenes.at(newSceneID);

        currentSceneIndex = index;
    }

    void SceneManager::init(EngineResource::ResourceManager &resourceManagerRef){
        resourceManager = &resourceManagerRef;

        std::vector<std::filesystem::path> files;
        for (auto& entry : std::filesystem::directory_iterator("../scenes"))
            if (std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".json")
                files.push_back(entry.path());

        std::sort(files.begin(), files.end());     
        
        if(files.empty()){
            addDefaultScene();
        } else{
            for(auto &file : files){
                deserializeScene(file.string());
            }
        }

        if(!sceneOrder.empty()) currentSceneIndex = 0; 
    }

    void SceneManager::serializeScene(Scene &scene, uint32_t sceneIndex){
        json sceneData;
        sceneData["name"] = scene.name;
        sceneData["id"] = scene.id;
        sceneData["objects"] = json::array();
        sceneData["cameras"] = json::array();

        //Loops through the scenes children and serializes them
        for(auto &node : scene.root.children){
            sceneData["objects"].push_back(serializeNode(node));
        }

        for(auto camera : scene.cameraManager.getCameras()){
            sceneData["cameras"].push_back(serialzeCamera(camera));
        }

        std::filesystem::create_directories(("../scenes"));
        std::ofstream file("../scenes/scene" + std::to_string(sceneIndex) + ".json");
        if(!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file");
        }

        file << sceneData.dump(4);
        file.close();
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

        for(auto &jsonCam : sceneData["cameras"]){
            EngineCamera::Camera *camera = deserializeCamera(jsonCam);
            scene->cameraManager.pushCamera(std::shared_ptr<EngineCamera::Camera>(camera));
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