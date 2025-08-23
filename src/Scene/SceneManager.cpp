#include "SceneManager.hpp"
#include <filesystem>
#include <fstream>
#include <memory>

#include "ObjectRegistry.hpp"
#include "nlohmann/json.hpp"

using namespace EngineObject;

namespace EngineScene{
    SceneManager::SceneManager(){};

    void SceneManager::addScene(){
        Scene scene = Scene::createScene();
        scene.initScene(true);
        scenes.push_back(std::move(scene));
    }

    void SceneManager::changeScenes(int indexChange){
        if(currentSceneIndex + indexChange >= scenes.size() || currentSceneIndex + indexChange < 0){
            return;
        }
        if(scenes[currentSceneIndex + indexChange].isInitialized == false){
            scenes[currentSceneIndex + indexChange].initScene(false);
        }

        currentSceneIndex+=indexChange;
    }

    void SceneManager::init(){
        std::vector<std::filesystem::path> files;
        for (auto& entry : std::filesystem::directory_iterator("../scenes"))
            if (std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".json")
                files.push_back(entry.path());

        if(files.empty()){
            addScene();
        }

        std::sort(files.begin(), files.end());

        for(auto &file : files){
            deserializeScene(file.string());
        }
    }

    json SceneManager::serializeObject(const Object &object){
        json jsonData;
        jsonData["type"] = object.type;
        jsonData["position"] = {object.position.x, object.position.y, object.position.z};
        jsonData["mesh"] = object.mesh.meshPath;
        jsonData["texture"] = object.texture.texturePath;
        jsonData["shader"] = {
            {"vert", object.shader.vertShader},
            {"frag", object.shader.fragShader}
        };

        return jsonData; 
    }
    
    Object* SceneManager::deserializeObject(const nlohmann::json& jsonData){
        Object* obj = ObjectRegistry::get().create(jsonData["type"], jsonData);

        return obj;
    }

    void SceneManager::serializeScene(Scene &scene, uint32_t sceneIndex){
        json sceneData;
        sceneData["objects"] = json::array();

        for(auto &obj : scene.objects){
            sceneData["objects"].push_back(serializeObject(*obj));
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

        Scene scene = Scene::createScene();

        for(auto &jsonObj : sceneData["objects"]){
            Object* object = deserializeObject(jsonObj);
            scene.objects.push_back(std::unique_ptr<Object>(object));
        }

        scenes.push_back(std::move(scene));
    }

    void SceneManager::saveScenes(){
        for(int i = 0; i < scenes.size(); i++){
            serializeScene(scenes[i], i);
        }
    }

    std::vector<Scene> &SceneManager::getScenes(){
        return scenes;
    }

    Scene &SceneManager::getCurrentScene(){
        return scenes[currentSceneIndex];
    }
}