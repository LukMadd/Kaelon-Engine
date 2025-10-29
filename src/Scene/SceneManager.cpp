#include "Scene/SceneManager.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "Camera/Camera.hpp"
#include "Core/ObjectRegistry.hpp"
#include "Core/ResourceManager.hpp"
#include "nlohmann/json.hpp"

#include "Serialization/Serialization.hpp"

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

        spatialPartitioner->reset();
        for(auto &object : getCurrentScene()->objects){
            //Helps prevent collision issues when switching back to a previously visited scene
            object->assignedToCell = false;
            object->cells.clear();
            object->updateCells();
        }
    }

    void SceneManager::init(EngineResource::ResourceManager &resourceManager, 
                            EnginePartitioning::Spacial_Partitioner *spatialPartitioner){
        this->resourceManager = &resourceManager;
        this->spatialPartitioner = spatialPartitioner;

        std::vector<std::filesystem::path> files;
        for(auto& entry : std::filesystem::directory_iterator(KAELON_SCENE_DIR))
            if(std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".json")
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
        sceneData["index"] = scene.index;
        sceneData["objects"] = json::array();
        sceneData["cameras"] = json::array();

        //Loops through the scenes children and serializes them
        for(auto &node : scene.root.children){
            sceneData["objects"].push_back(serializeNode(node));
        }

        for(auto camera : scene.cameraManager.getCameras()){
            sceneData["cameras"].push_back(serialzeCamera(camera));
        }

        std::filesystem::create_directories((KAELON_SCENE_DIR));
        std::ofstream file(std::string(KAELON_SCENE_DIR) + "/scene" + std::to_string(sceneIndex) + ".json");
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

        std::unique_ptr<Scene> scene = Scene::createScene(sceneData["index"], sceneData["name"]);

        scene->objects.clear();

        for(auto &jsonObj : sceneData["objects"]){
            SceneNode* node = deserializeNode(*scene, jsonObj);
            scene->root.addChild(node);
        }

        for(auto &jsonCam : sceneData["cameras"]){
            EngineCamera::Camera *camera = deserializeCamera(jsonCam);
            scene->cameraManager.pushCamera(std::shared_ptr<EngineCamera::Camera>(camera));
        }

        sceneOrder.push_back(scene->index);

        scenes[scene->index] = (std::move(scene));

        currentID++;
    }

    void SceneManager::deleteScene(Scene *scene){
        if(getScenes().size() == 1){
            std::cerr << "Warning: Cannot delete a scene when only one scene is present" << std::endl;
            return;
        }
        int sceneID = scene->index;

        scenes.erase(sceneID);

        auto it = std::find(sceneOrder.begin(), sceneOrder.end(), sceneID);
        if(it != sceneOrder.end()) {
            size_t removedIndex = std::distance(sceneOrder.begin(), it);
            sceneOrder.erase(it);

            if(currentSceneIndex > removedIndex){
                currentSceneIndex--;
            } else if(currentSceneIndex == removedIndex){
                currentSceneIndex = std::min(currentSceneIndex, (int)sceneOrder.size() - 1);
            }
        }


        std::string filename = std::string(KAELON_SCENE_DIR) + "scene" + std::to_string(sceneID) + ".json";
        if(std::filesystem::exists(filename)){
            std::filesystem::remove(filename);
        }
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
        if(sceneOrder.empty()){
            std::cerr << "(SceneManager::getCurrentScene()) Error: Scene order is empty! Returning nullptr" << std::endl;
            return nullptr;
        }
        int id = sceneOrder[currentSceneIndex];
        auto it = scenes.find(id);
        if(it == scenes.end()){
            std::cerr << "(SceneManager::getCurrentScene()) Error invliad currentSceneIndex! Returning nullptr" << std::endl;
            return nullptr;
        }

        return scenes.at(sceneOrder[currentSceneIndex]).get();
    }

    Scene* SceneManager::getScene(int id){
        auto it = scenes.find(id);
        if(it != scenes.end()) return it->second.get();
        return nullptr;
    }
}