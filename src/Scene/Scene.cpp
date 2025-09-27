#include "Scene.hpp"
#include "BaseObjects.hpp"
#include "Object.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineObject;

namespace EngineScene{
    Scene::Scene(const std::string &name, int index) : name(name), index(index){
        root = SceneNode();
        root.transform.position = glm::vec3(0.0f);
        root.transform.rotation = glm::quat(1.0f, 0, 0, 0);
        root.transform.scale = glm::vec3(1.0f);
        root.transform.localMatrix = glm::mat4(1.0f);
        root.transform.worldMatrix = glm::mat4(1.0f);
    }

    void Scene::initBaseScene(EngineResource::ResourceManager &resourceManager){
        cameraManager.checkIfCamerasEmpty();
        auto floor = std::make_unique<MeshObject>(glm::vec3( 0, 0, 0), "models/Crate1.obj");

        SceneNode* floorNode = new SceneNode();
        floorNode->object = floor.get();
        floorNode->transform.position = floor->getPosition();
        floorNode->transform.scale = glm::vec3(10.0f);
        floorNode->transform.scale.y = 0.5f;
        floor->node = floorNode;
        floor->material->setBaseColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
        floor->name = "Base_Scene_Floor";

        objects.push_back(std::move(floor));
        root.addChild(floorNode);   

        auto cube = std::make_unique<MeshObject>(glm::vec3( 0, 2, 0), "models/Crate1.obj");

        SceneNode* cubeNode = new SceneNode();
        cubeNode->object = cube.get();
        cubeNode->transform.position = cube->modelMatrix[3];
        cubeNode->transform.scale = glm::vec3(2.0f);
        cube->node = cubeNode;
        cube->material->setBaseColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
        cube->name = "Base_Scene_Cube";

        objects.push_back(std::move(cube));
        root.addChild(cubeNode);   

        root.update();
    }

    void Scene::addDefaultObject(){
        auto object = std::make_unique<MeshObject>(glm::vec3(0, 0, 0), "models/sphere.obj");

        SceneNode* objectNode = new SceneNode();
        objectNode->object = object.get();

        objectNode->transform.position = object->modelMatrix[3];
        objectNode->transform.scale = glm::vec3(0.1f);
        
        object->node = objectNode;
        object->material->setBaseColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
        object->name = "Default_Object";

        newObjects.push_back(object.get());

        objects.push_back(std::move(object));
        root.addChild(objectNode);

        root.update();
    }

    void Scene::removeObject(Object *object){
        auto nodeIt = std::find(object->node->parent->children.begin(), object->node->parent->children.end(), object->node);
        if(nodeIt != object->node->parent->children.end()){ //Don't think this is necessary but oh well
            object->node->parent->children.erase(nodeIt);
            object->node = nullptr;
        }

        auto newIt = std::find(newObjects.begin(), newObjects.end(), object);
        if(newIt != newObjects.end()){
            newObjects.erase(newIt);
        }

        object->cleanup(device);
        
        auto it = std::find_if(objects.begin(), objects.end(), [&](const std::unique_ptr<Object> &obj){
            return obj.get() == object;
        });
        if(it != objects.end()){
            objects.erase(it);
        }
    }

    std::vector<std::shared_ptr<Texture>> Scene::getSceneTextures(){
        std::vector<std::shared_ptr<Texture>> sceneTextures;
        for(auto &object : objects){
            for(auto &texture : object->material->getTextures()){
                if(texture->isValid()){
                    sceneTextures.push_back(texture);
                }
            }
        }
        return sceneTextures;
    }

    void Scene::cleanupObjects(){
        for(auto& obj : objects){
            obj->cleanup(device);
        }
        objects.clear();

        root.children.clear();
    }

    std::unique_ptr<Scene> Scene::createScene(int id, const std::string &name){
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(name, id);
        return scene;
    }

     void Scene::update(){
        root.update();
     }
}