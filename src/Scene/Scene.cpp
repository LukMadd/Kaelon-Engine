#include "Scene.hpp"
#include "BaseObjects.hpp"
#include "Object.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineObject;

namespace EngineScene{
    Scene::Scene(const std::string &name, int id) : name(name), id(id){
        root = SceneNode();
        root.transform.position = glm::vec3(0.0f);
        root.transform.rotation = glm::quat(1.0f, 0, 0, 0);
        root.transform.scale    = glm::vec3(1.0f);
        root.transform.localMatrix = glm::mat4(1.0f);
        root.transform.worldMatrix = glm::mat4(1.0f);
    }

    //Will probably be removed later, right now just used because there is nothing to add scenes
    void Scene::initScene(bool whichScene, EngineResource::ResourceManager &resourceManager){
        glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
        if(whichScene){
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    auto cube = std::make_unique<MeshObject>(pos, "models/Crate1.obj", "textures/crate_1.jpg");

                    SceneNode* node = new SceneNode();
                    node->object = cube.get();
                    node->transform.position = node->object->modelMatrix[3];
                    node->transform.scale = glm::vec3(0.5f);
                    cube->node = node;

                    objects.push_back(std::move(cube));
                    root.addChild(node);   

                    pos.x+=1.75;
                }
                pos.x = 0;
                pos.z+=1.5;
                pos.y+=1.5;
        }
        } else{
            auto sphere = std::make_unique<MeshObject>(pos, "models/sphere.obj", "textures/viking_room.png");
            SceneNode* node = new SceneNode();
            node->object = sphere.get();
            node->transform.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0,1,0));
            node->transform.scale = glm::vec3(1.0f);
            node->transform.position = glm::vec3(2,1,0);
            sphere->node = node;
            
            objects.push_back(std::move(sphere));
            root.addChild(node);
        }

        root.update();
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